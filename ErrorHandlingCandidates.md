# エラーハンドリング候補一覧

## 1. この資料の目的

`Common/LogSystem` のログシステムを利用して、エラー検知・記録・復旧処理を追加した方がよい箇所を、ソリューション全体から洗い出した一覧です。

今回、すでに対応済みとされている以下は、原則として追加候補から除外しています。

- `GameEngine/src/Application.cpp`
- `GameEngine/src/System/DeviceResources.cpp`

ただし、これらの呼び出し元・後続処理には、初期化結果を確認していない箇所が残っている可能性があるため、関連箇所は候補として残しています。

> 既存の `GameEngine/ErrorHandlingAudit.md` は GameEngine 内部の詳細監査資料です。本資料では、その内容を優先度付きの実装候補一覧として再整理し、`Projection`、`REngineTester`、`ComponentRegistry` も追加しています。

---

## 2. ログレベルと基本方針

| 状況 | 推奨ログ | 基本対応 |
|---|---|---|
| 起動に必須な初期化に失敗 | `LOG_FATAL` | ログをFlushしてアプリケーションを終了 |
| エンジン全体に影響する失敗 | `LOG_ERROR` | 呼び出し元へ失敗を返す、または安全に終了 |
| 一部アセット・描画機能だけの失敗 | `LOG_ERROR` | 対象機能を無効化し、他の処理を継続 |
| 任意参照・設定の欠落 | `LOG_WARN` | null、既定値、フォールバックで継続 |
| 非同期ロード待ち・通常の未成立状態 | `LOG_DEBUG` | 必要な場合のみ状態変化時に記録 |
| 正常な検索失敗・未接続状態 | 原則不要 | 毎フレームのログ出力を避ける |

`LOG_FATAL` は `FatalExit()` によって終了するため、単なるアセット欠落や任意の参照解決失敗には使用しない方が安全です。

ログには可能な限り次の情報を含めます。

- 対象ファイルまたはアセットパス
- Asset UUID、GameObject UUID、Component名
- 操作名
- HRESULTまたはWin32エラーコード
- 失敗したリソース名・シェーダー名
- シーン名
- 非同期Jobの識別情報

---

# 3. 最優先候補（P0）

## 3.1 Common/LogSystem自体

| 対象 | 確認内容 | 推奨対応 |
|---|---|---|
| `GameEngine/include/Common/LogSystem/Logger.h` / `src/Common/LogSystem/Logger.cpp` | LoggerのSink追加・Flush・Dispatchが失敗した場合の扱いがない | Sinkの例外・書き込み失敗をプロセス異常に発展させない方針を決める。最低限、Sink内部で失敗を握り潰さず、デバッグ出力などにフォールバックする |
| `GameEngine/include/Common/LogSystem/Logger.h` | `AddSink()` のSink生成、`push_back()`時の例外 | `std::bad_alloc`などをFatal扱いにするか、ログ初期化失敗として呼び出し元へ返せるAPIを検討 |
| `GameEngine/src/Common/LogSystem/FileLogSink.cpp` | ログファイルを開けない、書き込みに失敗、Flushに失敗 | パス、`errno`、ストリーム状態を記録。ファイルSinkが使えない場合もDebugLogSinkへ切り替えて処理を継続 |
| `GameEngine/src/Common/LogSystem/DebugLogSink.cpp` | `OutputDebugString`相当の出力失敗や巨大メッセージ | ログ出力自体がゲーム処理を停止させないようにする。ログメッセージのサイズ制限も検討 |
| `GameEngine/src/Common/LogSystem/DxLog.cpp` | D3D11 InfoQueueがnull、メッセージ取得失敗、バッファサイズ計算失敗 | D3Dデバッグ機能がない環境ではWarnで無効化し、エンジン本体を止めない |
| `GameEngine/include/Common/LogSystem/Logs.h` | `LOG_FATAL` は必ず終了する | Fatal対象を起動必須条件に限定する。機能単位の失敗には `LOG_ERROR` を使用 |
| `GameEngine/src/Common/LogSystem/Logger.cpp` | `std::format`、時刻・タイムゾーン取得、Sinkの `Write()` が例外を投げる可能性 | ロガーから例外を外へ漏らさない境界を設ける。特にエラー処理中のログ例外による二次障害を防ぐ |

### 優先理由

ログシステムが壊れていると、他の箇所にログを追加しても原因調査ができません。また、エラー処理中にログ処理が例外を投げると、元のエラーよりも深刻なクラッシュになる可能性があります。

---

# 4. 起動・エンジン初期化（P1）

## 4.1 エントリポイント

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/include/EntryPoint.h` | `CreateApplication()` がnull | `GetProps()`を呼ぶ前に検証し、`LOG_FATAL` |
| `EntryPoint.h` | `Application::Initialize()` の結果を確認していない | `Initialize()`をbool/Result型にして結果を確認。失敗時は原因を記録して終了 |
| `EntryPoint.h` | `ShowWindow`、`UpdateWindow`、メッセージループ開始後の初期化失敗 | Win32エラーコードを記録し、終了処理へ移行 |
| `EntryPoint.h` | `CoUninitialize()` が実行されない経路 | RAIIまたは終了経路を一本化 |
| `EntryPoint.h` | `CreateApplication()`や初期化処理が例外を投げる | 最上位の例外境界を設け、例外内容を記録して異常終了 |

ApplicationとDeviceResources本体にログを追加済みでも、初期化関数の戻り値を上位で確認しないと、失敗後にnullリソースを使って処理が続きます。

## 4.2 GameEngine初期化

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/GameEngine.cpp` | `SceneManager`、`AssetManager`、`Renderer`、`ShadowRenderer` の初期化失敗 | 各初期化結果を確認し、必須/任意を分けて `LOG_FATAL` または `LOG_ERROR` |
| `GameEngine/src/GameEngine.cpp` | `EngineInitializer::Initialize()` の二重実行・登録失敗 | 初期化済みフラグだけでなく、各登録処理の結果を検証 |
| `GameEngine/src/GameEngine.cpp` | ImGui初期化・コンテキスト作成失敗 | Editor機能だけ無効化し、ゲーム本体を継続できる場合は `LOG_ERROR` |
| `GameEngine/src/GameEngine.cpp` | `DefaultMeshManager`の標準メッシュ生成失敗 | 失敗したメッシュ名を記録。必須メッシュならFatal、任意メッシュなら該当描画を無効化 |
| `GameEngine/src/GameEngine.cpp` | 終了順序の不整合 | Renderer、AssetManager、Scene、ImGuiの依存順を明確化し、終了処理中のnull参照を防止 |
| `GameEngine/include/GameEngine.h` | 未初期化状態で`Update`、`Render`、`BeginFrame`が呼ばれる | assertだけに依存せず、状態検証＋`LOG_ERROR`＋安全なreturn |
| `GameEngine/src/GameController.cpp` | ゲーム側Controllerの取得失敗・更新中のnull参照 | 必須Controllerの欠落を初期化時に検出し、毎フレームのログは避ける |

---

# 5. アセット・ファイル・JSON（P1）

## 5.1 アセットロード

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/Assets/System/AssetLoaders.cpp` | Textureロード失敗後に成功状態へ上書きされる | 成功時だけ`success=true`にする。パス、HRESULT、拡張子を `LOG_ERROR` |
| `AssetLoaders.cpp` | Font生成、SpriteFont読み込み例外 | 例外を捕捉し、フォントパスと内容を記録。代替フォントへフォールバック |
| `AssetLoaders.cpp` | AudioEngine null、音声ファイル欠落、SoundEffect生成失敗 | AudioClipをFailed状態にし、音声以外のゲーム処理を継続 |
| `GameEngine/src/Assets/System/Shader/ShaderLoader.cpp` | シェーダーファイル不存在、コンパイル失敗、反射情報取得失敗 | シェーダー名、パス、コンパイラエラー、HRESULTを記録。代替シェーダーまたは描画スキップ |
| `GameEngine/src/Assets/System/Model/OBJLoader.cpp` | OBJを開けない、形式不正、頂点/面データ不足、MTL参照欠落 | ファイル行番号・モデル名を記録。モデル単位でFailedにする |
| `GameEngine/src/Assets/System/Model/MtlFileLoader.cpp` | MTLを開けない、数値変換失敗、テクスチャ参照欠落 | MTL名と行番号を記録。該当マテリアルまたはテクスチャだけ無効化 |
| `GameEngine/src/Assets/System/Material/MaterialLoader.cpp` | JSON構文・必須キー・型が不正 | JSON例外を捕捉し、マテリアルパスとキー名を記録 |
| `GameEngine/src/Assets/System/Material/MaterialSaver.cpp` | 出力ファイルを開けない、書き込み失敗 | `ofstream`状態を検証し、保存成功扱いにしない |
| `GameEngine/src/Assets/System/AssetSaver.cpp` | Prefab/Property保存時のnullアセット、JSON生成失敗、書き込み失敗 | `pAsset`、パス、AssetTypeを検証。保存APIをbool/Result化 |
| `GameEngine/src/Assets/System/Model/CreatePrefabFromModelData.cpp` | モデルからPrefabを生成できない、子メッシュ生成失敗 | 生成対象名を記録し、部分生成されたPrefabを公開しない |
| `GameEngine/src/Assets/Managers/AssetAsyncLoader.cpp` | 非同期Jobの例外、Jobキャンセル、ロード失敗 | Job ID、UUID、パス、AssetType、失敗理由を記録。Failed状態を確実に設定 |
| `GameEngine/src/Assets/Managers/AssetManager.cpp` | `job.success`や`job.asset`がfalse/nullのまま黙ってcontinue | 失敗を記録し、旧アセット維持またはエラーハンドルへ遷移 |
| `AssetManager.cpp` | Creator/Saver/Loader未登録、未知AssetType、拡張子取得失敗 | `find() == end()`を確認し、無効イテレータ参照を防ぐ |
| `GameEngine/src/Assets/Managers/AssetRegistry.cpp` | Handleのindex、世代、スロットが不正 | 範囲・世代・有効性を検証し、UUID/Handleを記録 |
| `GameEngine/src/Assets/Managers/AssetDataBase.cpp` | `recursive_directory_iterator`の例外、ルート不存在、補助ファイル破損 | ディレクトリパスとfilesystemエラーを記録。走査できる範囲だけ継続 |
| `GameEngine/src/Assets/Managers/AuxFileRepository.cpp` | auxファイルの開閉、JSON構文、必須キー、書き込み失敗 | 既存のaux情報を壊さず、失敗時は旧情報を保持 |
| `GameEngine/src/Assets/Managers/AssetTypeManager.cpp` | 未対応拡張子、登録済み型の不一致 | 未対応はWarn、ロード処理中の不一致はError |
| `GameEngine/src/Assets/Managers/DefaultAssetManager.cpp` | 標準アセットの登録・取得失敗 | 起動必須の標準アセットのみFatal、その他は代替リソースへフォールバック |
| `GameEngine/src/Assets/Types/Models/DefaultMeshManager.cpp` | 標準Mesh生成、GPUバッファ作成失敗 | メッシュ名とHRESULTを記録し、使用箇所で描画をスキップ |
| `GameEngine/src/Assets/Types/Vertex/DynamicVertexBuffer.cpp` | Device/Context/Buffer null、`CreateBuffer`、`Map`失敗 | HRESULTと頂点数・Stride・容量を記録。`Write`は0件など明確な失敗値を返す |

## 5.2 JSON・エディターのロード/セーブ

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/Editor/Loader/ObjectLoader.cpp` | 必須キー、JSON型、配列サイズ、UUIDが不正 | JSON例外を捕捉し、ファイル、GameObject UUID、プロパティ名、型名を記録 |
| `ObjectLoader.cpp` | `ComponentFactory::Create()` がnull | コンポーネント名・GameObject UUID・シーンパスを記録し、該当Componentだけスキップ |
| `ObjectLoader.cpp` | 子Object、Transform、RectTransform生成失敗 | 子Object単位でロードを中断し、Scene全体を破壊しない |
| `GameEngine/src/Editor/Saver/ObjectSaver.cpp` | nullのGameObject、Scene、ObjectManager、PropertyObject | `LOG_ERROR`を出し、空JSONを成功値として返さない |
| `ObjectSaver.cpp` | `ofstream`を開けない、書き込み失敗、ディスク容量不足 | 保存APIをbool/Result化。成功後だけ編集状態を保存済みにする |
| `GameEngine/src/Editor/SceneEditor.cpp` | `SaveSceneToFile()`の結果未確認 | 保存結果を確認し、失敗時は編集内容を保持してUI通知＋`LOG_ERROR` |
| `GameEngine/src/Common/OpenFileDialog.cpp` | COMダイアログ生成、表示、選択結果、パス取得失敗 | HRESULT、Win32エラー、キャンセルとエラーを区別。キャンセルは通常ログ不要 |

---

# 6. 描画・GPUリソース（P1）

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/System/Render/RenderTarget.cpp` | Texture2D、RTV、SRV、DSV生成失敗 | width、height、format、リソース名、HRESULTを記録。対象RenderTargetを無効化 |
| `GameEngine/src/System/Render/RenderInfoManager.cpp` | ConstantBuffer生成、`Map`、`Unmap`失敗 | バッファ名、サイズ、HRESULT、フレーム番号を記録。該当Drawをスキップ |
| `GameEngine/src/Renderer/Shadow/ShadowMap.cpp` | サイズ0、Device null、Texture/SRV/DSV生成失敗 | Shadowだけを無効化し、通常描画を継続 |
| `GameEngine/src/Renderer/Shadow/ShadowRenderer.cpp` | DepthStencilState生成失敗、ShadowMap未初期化 | ShadowRendererを無効化。通常Rendererへ影響させない |
| `GameEngine/src/System/Render/SamplerList.cpp` | `CreateSamplerState`失敗 | SamplerType、Filter、AddressMode、HRESULTを記録 |
| `GameEngine/src/System/Render/RasterizerStateList.cpp` | `CreateRasterizerState`失敗 | assertだけに依存せず、Error＋null/無効状態で安全に停止 |
| `GameEngine/src/System/Render/BlendStateList.cpp` | BlendState生成・取得失敗 | State名と設定値を記録し、既定BlendStateへフォールバック |
| `GameEngine/src/System/Render/DepthStencilStateList.cpp` | DepthStencilState生成・取得失敗 | 影・深度処理のどの機能を無効化したか記録 |
| `GameEngine/src/System/Render/InputLayoutCache.cpp` | VertexShader null、反射情報不正、InputLayout生成失敗 | Shader UUID/パス、Vertex layout、HRESULTを記録し、Mesh描画をスキップ |
| `GameEngine/include/Renderer/Renderer.h` / `src/Renderer/Renderer.cpp` | 未初期化状態でBegin/Draw/End | Releaseビルドでも状態を検証し、Error＋安全なreturn |
| `GameEngine/src/Renderer/Command/DrawCommandContainer.cpp` | DrawCommandのnull、容量超過、無効なコマンド | コマンド発行元と種類を記録し、該当コマンドだけ破棄 |
| `GameEngine/src/Renderer/Command/DrawCommandExecutor.cpp` | リソースnull、無効Shader、無効InputLayout | Draw単位でスキップし、毎フレーム同じエラーを出さない抑制を入れる |
| `GameEngine/src/Renderer/RenderProxy.cpp` | RenderProxyが無効、対象Component/Assetが消滅 | Proxy IDと所有Objectを記録し、Proxyを登録解除 |
| `GameEngine/src/Renderer/Command/Publisher/*.cpp` | 必須描画情報・Mesh・Texture・Fontの欠落 | 発行前に検証し、描画コマンドを生成しない |

---

# 7. シーン・GameObject・Component（P2）

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/Scene/SceneManager.cpp` | 未登録シーン、シーンファイルロード失敗 | ユーザー操作ミスはWarn、初期シーン失敗はError。現在シーンを維持 |
| `GameEngine/src/Scene/ObjectFactory.cpp` | Prefab null、GameObject生成失敗、AddComponent失敗 | 明示的なInstantiate失敗はError。生成途中のObjectを公開しない |
| `GameEngine/src/Scene/Scene.cpp` | UUID参照解決失敗、必須Object/Component欠落 | UUID、参照元、対象Componentを記録。任意参照はWarn、必須はError |
| `GameEngine/src/Scene/ObjectFinder.cpp` | 検索結果null | null自体が正常な場合があるため、ObjectFinderではログを出さず、必須と判断する呼び出し側で記録 |
| `GameEngine/src/Components/World/Transform/Transform.cpp` | 自分自身・子孫を親に設定、親null、行列計算のNaN/Inf | 循環はWarn/Error。異常な行列やQuaternionは既定値へフォールバック |
| `GameEngine/src/GameObject/GameObject.cpp` | Component追加・削除、親子付け、ObjectManager登録失敗 | Object UUID、Component名、所有Sceneを記録。部分生成をロールバック |
| `GameEngine/src/GameObject/ComponentContainer.cpp` | 型不一致、重複Component、取得null | 必須Componentの取得失敗をError。任意ComponentはWarnまたは無出力 |
| `GameEngine/src/Editor/Loader/ComponentFactory.cpp` | 未登録Component名、登録関数がnull | Component名とProjectを記録。登録処理の重複もWarn |
| `GameEngine/src/Managers/CollisionMap.cpp` | null Collider、重複登録、登録されていない組み合わせ | null入力はError。重複はWarn。未登録組み合わせのfalseは通常ログ不要 |
| `GameEngine/src/Managers/2DManagers/*` | 空間分割Cell、Collider、Hit判定へのnull | 入力境界で一度だけError。毎フレームの衝突なしはログ不要 |
| `GameEngine/src/Managers/3DManagers/*` | Ray、Collider、SpaceDivisionの無効入力 | Ray origin/direction、Collider IDを記録し、無効な問い合わせはfalseを返す |
| `GameEngine/src/Managers/CameraManager.cpp` | Active Camera null、カメラ登録/解除の不整合 | フレーム描画前に検証し、既定カメラへフォールバック |
| `GameEngine/src/Components/World/Camera/*.cpp` | Transform、Target、Projectionパラメータの不正 | Target未設定はWarn、必須TransformはError。既定Projectionへフォールバック |
| `GameEngine/src/Managers/Renderer/LightManager.cpp` | LightのTransform欠落、Light数上限超過 | 該当Lightだけ無効化。Light情報とGameObjectを記録 |
| `GameEngine/src/Components/World/Light/LightComponent.cpp` | 色、強度、方向、範囲などの不正値 | 範囲外値を補正し、設定元Objectと補正内容をWarn |

---

# 8. UI・音声・入力（P2）

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/src/Components/UI/Canvas/Canvas.cpp` | RectTransform、GameObject、ImageUIなど必須Component欠落 | Canvas単位で描画・ヒットテストを無効化し、Error |
| `GameEngine/src/Components/UI/ImageUI.cpp` | AssetManager null、Texture未ロード、無効Handle | 設定済みアセットのロード失敗はWarn/Error。非同期待ちはDebug |
| `GameEngine/src/Components/UI/TextUI.cpp` | Font未ロード、文字コード・文字列変換失敗 | Font名とTextUI所有Objectを記録し、既定フォントへフォールバック |
| `GameEngine/src/Components/UI/Canvas/CanvasScaler.cpp` | 画面サイズ0、基準解像度0、計算結果NaN | 入力値を検証し、既定解像度へフォールバック |
| `GameEngine/src/Components/World/MeshRenderer.cpp` | AssetManager、MeshFilter、Mesh、Materialの欠落 | 必須依存欠落はError、ロード待ちはDebug、ロード失敗はError |
| `GameEngine/src/Components/Both/Sounds/AudioSource.cpp` | AudioClip、AudioEngine、SoundEffectInstance生成失敗 | AudioSourceだけ無効化。再生要求のたびにログを出さない |
| `GameEngine/src/Managers/SoundManager.cpp` | AudioEngine null、AudioListener未登録、音量値不正 | Listener未登録は初回だけWarn。音声機能を無効化してゲームを継続 |
| `GameEngine/src/Components/World/Sounds/AudioListener.cpp` | Listener重複、アクティブListener不在 | 重複はWarn、選択ルールを明確化 |
| `GameEngine/src/Input/InputSystem.cpp` | 未登録入力名、壊れたBinding、キー変換失敗 | Binding読込時に一度だけWarn。毎フレーム問い合わせではログを出さない |
| `GameEngine/src/Input/PadInput.cpp` | ゲームパッド未接続、接続切断、振動設定失敗 | 未接続は正常状態。接続状態の変化時だけDebug/Info。API失敗時のみWarn |

---

# 9. タイマー・イベント・Editor（P2〜P3）

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `GameEngine/include/Timer/StepTimer.h` | 異常な経過時間、更新回数上限超過、例外送出 | 例外がアプリ最上位まで漏れない境界を設ける。フレームをスキップするか終了するか方針化 |
| `GameEngine/src/Timer/GameTimer.cpp` | delta timeがNaN/Inf、時間倍率が負値・極端 | Warnを状態変化時に出し、既定値へ補正 |
| `GameEngine/include/Common/EventBus.h` | null購読者、イベント中の購読解除、コールバック例外 | コールバック単位で例外を捕捉し、イベント名と購読者型を記録 |
| `GameEngine/src/Editor/SceneEditor.cpp` | 編集対象Scene null、選択Object消滅 | UI操作を無効化して継続。頻繁な操作エラーはWarnを抑制 |
| `GameEngine/src/Editor/Editor/Windows/Project/AssetFileDrawer.cpp` | ファイル削除・リネーム・ドラッグ操作失敗 | パス、操作、Win32/filesystemエラーを記録。UI上で失敗を通知 |
| `GameEngine/src/Editor/Editor/Windows/Project/ProjectWindowPopUp.cpp` | 不正パス、重複アセット、作成/保存失敗 | 操作単位のError。編集状態を失敗前のまま維持 |
| `GameEngine/src/Editor/Editor/Windows/Inspector/PropertyOnInspector.cpp` | null Property、型変換失敗、範囲外入力 | Property名・型名を記録し、変更を適用しない |
| `GameEngine/src/Editor/Editor/Windows/LogWindow.cpp` | ログ一覧の競合、古いSink参照、表示中のログ削除 | Loggerの寿命とスレッド同期を確認。UI表示失敗で本体を停止させない |
| `GameEngine/src/Editor/SceneEditor.cpp` | Gizmo、選択対象、編集対象の参照切れ | 対象を選択解除しWarn。毎フレーム出力しない |

---

# 10. Projection（実ゲーム側）

## 10.1 初期化・シーン登録

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `Projection/Game/Game.cpp` | ゲーム固有ComponentのFactory登録失敗・重複登録 | 登録APIをbool/Result化し、Component名とProjectを記録 |
| `Projection/Game/Game.cpp` | `RegisterScene()`失敗、シーンファイル不存在 | シーン名・パスをError。開始シーンに設定しない |
| `Projection/Game/Game.cpp` | `SetStartScene("Title")`対象が未登録 | 初期化時に確認し、起動不能ならError/Fatalを選択 |
| `Projection/Game/GameInitializer.cpp` | ゲーム固有初期化、アセット、イベント登録失敗 | 各登録処理を分割して結果確認。失敗対象を特定 |
| `Projection/Game/ComponentRegister.cpp` | Componentの登録漏れ、Factory生成関数null | 登録数・登録名を検証し、重複/未登録をErrorまたはWarn |

## 10.2 ゲーム固有処理

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `Projection/Game/Common/ChangeSceneComponent.cpp` | 存在しないSceneへの遷移要求、遷移対象null | Scene名を記録。現在Sceneを維持 |
| `Projection/Game/GamePlayScene/PlaySceneManager.cpp` | Stage、Player、Goalなど必須Object未検出 | Scene名・Object名・UUIDをError。該当ゲームプレイを無効化 |
| `Projection/Game/GamePlayScene/Player/Player.cpp` | 必須Collider、Transform、Cameraの欠落 | Player生成時に検証し、部分生成を公開しない |
| `Projection/Game/GamePlayScene/ChangeDimention/ChangeColliderComponent.cpp` | Collider生成・検索失敗、`nullptr`返却 | 呼び出し元がnullを許容するか明確化。必須ならError、任意ならWarn |
| `Projection/Game/GamePlayScene/ChangeDimention/DimentionManager.cpp` | 次元切り替え対象、Collider、Scene状態の不整合 | 切り替え前後の状態を検証し、失敗時は元の次元へ戻す |
| `Projection/Game/GamePlayScene/ChangeDimention/DepthCorrection.cpp` | 深度値、Projection行列、対象Transformの不正 | NaN/Infと範囲を検証し、既定値へフォールバック |
| `Projection/Game/GamePlayScene/Camera/*.cpp` | カメラTarget、Projection値、対象Objectの欠落 | Target未設定はWarn、必須対象欠落はError。既定カメラへ戻す |
| `Projection/GameLib/Colliders/*.cpp` | 頂点数不足、退化ポリゴン、null Collider | 入力が不正な場合はfalseを返し、登録/ロード時に一度だけError |
| `Projection/GameLib/UI/DialogController.cpp` | Dialogの生成・遷移・参照解決失敗 | Dialog名と状態を記録し、安全な画面へ戻す |
| `Projection/GameLib/Transition/SlideTransition.cpp` | Transition対象null、時間値0/負値、補間値異常 | Transitionをキャンセルして現在Sceneを維持 |
| `Projection/GameLib/GameMath/*.cpp` | Easing設定値の範囲外、Quaternion生成入力不正 | 設定ファイル由来の値を検証しWarn、既定値へ補正 |

Projection側はGameEngineのログヘッダーを直接利用できる構成ですが、ゲーム固有の登録・シーン・必須Objectの失敗をエンジン側だけで検知することはできません。

---

# 11. REngineTester

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `REngineTester/Game/Game.cpp` | テスト用Componentの登録失敗・重複 | 登録APIの結果を検証。Component名を記録 |
| `REngineTester/Game/Game.cpp` | `Title`、`GUITest`シーンのファイル不存在・登録失敗 | シーン名とパスを記録。開始シーン設定前に存在確認 |
| `REngineTester/Game/Game.h` | `CreateApplication()`がnull、テスト用設定値の不正 | エンジンのEntryPoint側で共通検証。テスト固有設定も初期化時に検証 |
| `REngineTester/Game/Components/*.cpp` | テストComponentの依存Component・Asset欠落 | テスト対象を無効化して、原因をComponent名付きで記録 |
| `REngineTester`のシェーダー | コンパイル失敗、InputLayout不一致 | ShaderLoader/Renderer側で検出し、シェーダーパスとエラーを記録 |

テストプロジェクトの不備は本番ゲームの不備と区別できるよう、ログにプロジェクト名または対象シーンを含めます。

---

# 12. ComponentRegistry

`ComponentRegistry` はGameEngineのログシステムをリンクしていないため、そのまま `LOG_ERROR` を使うには共通ログライブラリへの依存追加が必要です。現時点では、`std::cerr`、終了コード、またはログシステムの共有化を検討します。

| 対象 | 失敗ケース | 推奨対応 |
|---|---|---|
| `ComponentRegistry/main.cpp: GetCurrentModulePath()` | `GetModuleFileNameW()`失敗、戻り値0、Win32エラー | `GetLastError()`を記録し、空パスを返さない |
| `ComponentRegistry/main.cpp: HasProjectFile()` | directory iterator例外、アクセス拒否、対象パスがnull | filesystem例外を捕捉し、パスとエラー内容を出力 |
| `ComponentRegistry/main.cpp: CheckHeaderFile()` | ヘッダを開けない、読み取り失敗 | パスとストリーム状態を出力。空の結果と読み込み失敗を区別 |
| `CheckHeaderFile()` | 正規表現の構築・解析失敗、想定外の宣言形式 | regex例外を捕捉し、対象行番号を出力 |
| `CheckHeaderFile()` | `std::getline()`途中の読み込み失敗 | EOFとI/Oエラーを区別し、ファイルパス・行番号を出力 |
| `main()` | `CheckHeaderFile()`の結果未検証、出力先への書き込み失敗 | 結果件数を検証し、処理成功/失敗を終了コードへ反映 |
| `ComponentRegistry`の自動生成処理 | 生成先の書き込み、既存ファイル上書き、構文不正 | 一時ファイルへ出力して成功後に置換。失敗時に既存ファイルを保持 |

現状の`main()`では実際の自動走査処理の大部分がコメントアウトされているため、まずツールとしての入出力失敗を明確にするのが優先です。

---

# 13. エラー処理を追加しない方がよいケース

次の状態は、発生自体が異常とは限らないため、毎回 `LOG_ERROR` を出さない方がよいです。

- ゲームパッド未接続
- 非アクティブなGameObject/Component
- UIのRaycast対象外
- 任意参照の検索結果がnull
- 未登録の衝突組み合わせ
- Scene遷移中の追加遷移要求
- 非同期アセットロード待ち
- 任意Texture/Font未設定
- ShadowMap無効時でも通常描画を継続できる状態
- ユーザーによるファイルダイアログのキャンセル
- ObjectFinderで対象が見つからない状態

ただし、「必須」と宣言された参照、設定、アセットが欠落した場合はWarn/Errorへ昇格します。

---

# 14. 実装優先順位

## 第1段階：ログ基盤

1. FileLogSinkのファイルオープン・書き込み失敗を処理
2. Logger/Sinkからの例外漏れを防止
3. `LOG_FATAL`を起動必須条件に限定
4. HRESULT、Win32エラー、パスの出力形式を統一
5. 同一エラーの毎フレーム重複出力を抑制

## 第2段階：初期化結果の伝播

1. `GameEngine::Initialize()`の結果を呼び出し元へ返す
2. SceneManager、AssetManager、Renderer、ShadowRendererの初期化結果を確認
3. Projection/REngineTesterのシーン登録結果を確認
4. 必須機能と任意機能を分類

## 第3段階：アセットとJSON

1. AssetLoaders、ShaderLoader、OBJ/MTL、MaterialLoader
2. AssetManager、AssetRegistry、AssetDataBase、AuxFileRepository
3. ObjectLoader/ObjectSaver
4. Load/Save APIのboolまたはResult化

## 第4段階：GPU・描画

1. RenderTarget、RenderInfoManager、DynamicVertexBuffer
2. InputLayout、Sampler、Blend、Rasterizer、DepthStencil
3. ShadowMap/ShadowRenderer
4. DrawCommandのnull・無効リソース

## 第5段階：Scene・Component・UI・音声

1. SceneManager、ObjectFactory、ComponentFactory
2. GameObject/Transform/参照解決
3. Canvas、ImageUI、TextUI、MeshRenderer
4. AudioSource、SoundManager、InputSystem

## 第6段階：利用側・ツール

1. Projection固有のシーン・必須Object・遷移
2. REngineTesterの登録・シーン
3. ComponentRegistryのファイルI/O・終了コード

---

# 15. 追加実装時の注意点

- ログを追加するだけでなく、失敗後に処理を続行してよいかを決める。
- `void`関数で失敗を表現できない場合は、boolまたはResult型への変更を検討する。
- `assert`だけではReleaseビルドで消えるため、実行時チェックを追加する。
- nullチェックは呼び出し箇所ごとに乱立させず、初期化境界で必須依存を検証する。
- 非同期処理では、失敗Jobを黙って破棄せず、Failed状態または旧アセット維持へ遷移させる。
- `LOG_ERROR`を毎フレーム呼び出すとログ自体が問題になるため、状態変化時・対象単位で抑制する。
- パス、UUID、Component名、Scene名を必ず組み合わせ、後から対象を特定できるようにする。
- エラー処理中にさらにエラーが起きても、元のエラー情報が失われないようにする。

---

# 16. 参照資料

- `GameEngine/ErrorHandlingAudit.md`
- `GameEngine/include/Common/LogSystem/Logs.h`
- `GameEngine/include/Common/LogSystem/Logger.h`
- `GameEngine/src/Common/LogSystem/Logger.cpp`
- `GameEngine/src/Common/LogSystem/LogFatal.cpp`
- `GameEngine/include/EntryPoint.h`
- `GameEngine/src/GameEngine.cpp`
- `GameEngine/src/Scene/SceneManager.cpp`
- `GameEngine/src/Assets/Managers/AssetManager.cpp`
- `GameEngine/src/Editor/Loader/ObjectLoader.cpp`
- `GameEngine/src/Editor/Saver/ObjectSaver.cpp`
- `Projection/Game/Game.cpp`
- `REngineTester/Game/Game.cpp`
- `ComponentRegistry/main.cpp`
