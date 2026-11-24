# GameEngine エラーハンドリング調査報告書

## 調査概要

GameEngine配下の非ThirdParty C++コードを対象に、既存のログシステムを利用できる箇所、失敗検知が不足している箇所、ログだけでなく復旧処理が必要な箇所を調査した。

- 対象: `GameEngine/include`、`GameEngine/src`
- 除外: `GameEngine/src/ThirdParty`
- 対象規模: 約374個のC++ソースファイル
- 主な確認項目: `LOG_*`、`assert`、例外、HRESULT、ポインタ、ファイルI/O、JSON、アセットロード、GPUリソース生成、失敗戻り値

## 優先度

| 優先度 | 意味 |
|---|---|
| P0 | ログシステム自体の修正が必要 |
| P1 | 起動不能・描画不能・データ破損につながる |
| P2 | 該当機能だけ停止・スキップすべき |
| P3 | 通常の未成立状態。基本的にログ不要 |

---

# P0: ログシステム自体

## Loggerのatomic初期化

対象: `GameEngine/include/Common/LogSystem/Logger.h`

以下のメンバーが明示的に初期化されていない。

```cpp
std::atomic<uint64_t> m_frameCount;
std::atomic<LogLevel> m_minLevel;
```

### リスク

- ログのフレーム番号が不定になる可能性
- 最低ログレベルが不定になる可能性
- ログが無視される、または大量に出力される可能性

### 推奨

- `m_frameCount{ 0 }`
- `m_minLevel{ LogLevel::Info }`

などを明示的に初期化する。

## LOG_FATALのFlush呼び出し

対象: `GameEngine/include/Common/LogSystem/Logs.h:36-37`

`Flush()`は非staticメンバー関数だが、以下のようにクラス名経由で呼び出している。

```cpp
RHEngine::Logger::Flush();
```

Loggerのインスタンス経由で呼び出す必要がある。

```cpp
RHEngine::Logger::Instance().Flush();
```

`LOG_FATAL`はFatalログ、Flush、`__debugbreak()`を行うため、致命的な初期化失敗などに限定して使用する。

---

# P1: 起動・アプリケーションライフサイクル

## EntryPoint.h

対象: `GameEngine/include/EntryPoint.h`

### CPUサポート確認 `:40`

```cpp
if (!DirectX::XMVerifyCPUSupport()) return 1;
```

CPUサポート不足を`LOG_FATAL`で記録する。

### COM初期化 `:41-43`

```cpp
HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
if (FAILED(hr)) return 1;
```

HRESULTとCOM初期化失敗を記録する。起動に必須なのでFatal相当。

### Application生成 `:46-47`

```cpp
g_app.reset(RHEngine::CreateApplication());
const auto& props = g_app->GetProps();
```

`CreateApplication()`がnullを返した場合にクラッシュする。生成結果を検証し、失敗時はFatalログを出す。

### ウィンドウクラス登録 `:68`

```cpp
if (!RegisterClassExW(&wcex)) return 1;
```

`GetLastError()`、ウィンドウクラス名、失敗理由を記録する。

### ウィンドウ生成 `:81-82`

```cpp
if (!hwnd) return 1;
```

`GetLastError()`、ウィンドウサイズ、タイトルを記録する。

## DeviceResources.cpp

対象: `GameEngine/src/System/DeviceResources.cpp`

確認対象:

- 最低Feature Levelの不正
- ハードウェアアダプターの欠落
- D3D11デバイス生成失敗
- WARPフォールバック失敗
- DXGI Factory生成失敗
- SwapChain生成・Resize失敗
- デバイスロスト

### 推奨ログ

| 状況 | ログ・対応 |
|---|---|
| Feature Level設定不正 | `LOG_FATAL` |
| ハードウェアデバイスなし | `LOG_ERROR`または`LOG_FATAL` |
| WARPへフォールバック | `LOG_WARN` |
| WARPも失敗 | `LOG_FATAL` |
| デバイスロスト | `LOG_ERROR`後に再作成 |
| オプション機能非対応 | `LOG_WARN`、機能無効化 |

ログにはHRESULT、要求Feature Level、実際のFeature Level、アダプター名を含める。

## Application.cpp

対象: `GameEngine/src/Application.cpp`

### デバイスリソース取得 `:27-30`

`GetDeviceResources()`の結果を検証する。nullの場合はFatalログ。

### WindowSizeChanged

以下を検証する。

- `width <= 0`
- `height <= 0`
- DeviceResourcesのnull
- Resize処理の失敗

### Clear `:129-153`

以下のnullを検証する。

- DeviceContext
- RenderTargetView
- DepthStencilView

異常時は`LOG_ERROR`を出して描画処理をスキップする。

---

# P1: アセット・ファイル・JSON

## AssetLoaders.cpp

対象: `GameEngine/src/Assets/System/AssetLoaders.cpp`

### TextureLoader `:60-73`

失敗時に`result.success = false`へ設定した後、関数末尾で`result.success = true`に上書きされる明確な不具合がある。

### 影響

- 失敗テクスチャが正常アセットとして扱われる
- nullのShaderResourceViewが後段へ渡る
- 描画時にクラッシュまたは無表示になる

### 推奨

- 成功時だけ`success=true`にする
- 失敗時に`LOG_ERROR`
- パス、拡張子、HRESULTを記録
- エラーアセットまたは無効ハンドルを返す

### FontLoader

対象: `:77-101`

`SpriteFont`生成時の例外を捕捉する。ファイルパスと例外内容を記録し、失敗時はエラー用フォントへフォールバックする。

### AudioClipLoader

対象: `:115-130`

以下を検証する。

- AudioEngineのnull
- 音声ファイルの存在
- 音声形式
- `SoundEffect`生成例外

失敗時は`LOG_ERROR`、AudioClipをFailed状態にする。

### PrefabLoader

PrefabのJSON構造を検証する。破損Prefabは`LOG_ERROR`を出して該当Prefabだけ無効化する。

## AuxFileRepository.cpp

対象: `GameEngine/src/Assets/Managers/AuxFileRepository.cpp`

### SaveAux `:33-45`

以下の失敗を検出する。

- ファイルを開けない
- ディレクトリ不存在
- 書き込み権限不足
- `ofs.good()`がfalse
- ディスク容量不足

パス、Asset UUID、AssetType、ストリーム状態を`LOG_ERROR`で記録する。保存APIはboolまたはResult型にする。

### LoadAux `:47-74`

以下を検証する。

- ファイル存在
- JSON構文
- `UUID`キー
- `AssetType`キー
- `SubAssets`キー
- 各フィールドの型

JSON例外を捕捉し、既存の`aux`を壊さずにロードを中断する。

## AssetManager.cpp

対象: `GameEngine/src/Assets/Managers/AssetManager.cpp`

### Update `:44-85`

```cpp
if (!job.success || !job.asset) continue;
```

非同期ロード失敗を黙って捨てている。以下をログに記録する。

- Jobのハンドル
- UUID
- ファイルパス
- AssetType
- 失敗理由

失敗アセットをFailed状態にするか、旧アセットを維持する。

### LoadFromUUID `:87-124`

以下を区別する。

- `UUID_NONE`
- 未登録UUID
- MainAsset欠落
- サブアセット欠落
- Registry登録失敗

通常の未登録UUIDは`LOG_DEBUG`または`LOG_WARN`、ロード処理中の欠落は`LOG_ERROR`とする。

### Create `:127-164`

以下の失敗を検出する。

- 未対応AssetType
- 拡張子取得失敗
- Creator未登録
- Saver未登録
- Asset生成失敗
- 保存失敗
- `ScanOnceFile`失敗

`m_creators.find(idx)`の結果が`end()`か確認せずに参照しているため、無効イテレータ参照の危険がある。

## AssetRegistry.cpp

対象: `GameEngine/src/Assets/Managers/AssetRegistry.cpp`

### Replace `:92-108`

`index`の範囲チェックがない。

- index範囲
- 世代
- スロット有効性

を確認し、異常時は`LOG_ERROR`を出す。

---

# P1: エディターの保存・読み込み

## ObjectLoader.cpp

対象: `GameEngine/src/Editor/Loader/ObjectLoader.cpp`

### JSON検証不足

以下の処理はJSONの型とキーが正しいことを前提にしている。

- `json.get<int>()`
- `json[0]`
- `json["size"]`
- `json["array"][i]`
- `json["Components"]`
- `json["Children"]`
- `js["Type"]`
- `js["Data"]`

JSON例外を捕捉し、ファイルパス、GameObject UUID、プロパティ名、型名を記録する。

### ComponentFactory失敗

未知のコンポーネント名でnullが返る。コンポーネント名、対象GameObject UUID、シーンファイルを`LOG_ERROR`に記録し、そのコンポーネントだけスキップする。

### 子オブジェクト生成 `:177-201`

以下の生成結果を検証する。

- `pScene`
- `Generate()`結果
- `Transform`
- `RectTransform`

失敗時は子オブジェクト単位でロードを中断する。

### Property値

`property.value`がnullの場合、型変換時にクラッシュする。プロパティ名・型名を記録し、そのプロパティだけ読み込みをスキップする。

## ObjectSaver.cpp

対象: `GameEngine/src/Editor/Saver/ObjectSaver.cpp`

### nullポインタ

以下を検証する。

- `SaveObject(GameObject*)`
- `SaveObjectManager(ObjectManager*)`
- `SaveScene(Scene*)`
- `SavePropertyToFile(PropertyObject*)`

null時は`LOG_ERROR`を出し、空JSONを返さない。

### ファイル保存 `:237-280`

対象:

- `SaveObjectToFile`
- `SaveSceneToFile`
- `SavePropertyToFile`

ファイルを開けない、書き込み失敗、`ofs.fail()`、ディスク容量不足を検出する。保存成功後だけ編集状態を保存済みにする。

## SceneEditor.cpp

対象: `GameEngine/src/Editor/SceneEditor.cpp:201-203`

`SaveSceneToFile()`の結果を確認していない。保存失敗時は編集内容を保持し、UI通知と`LOG_ERROR`を行う。

---

# P1: 描画・GPUリソース

## RenderInfoManager.cpp

対象: `GameEngine/src/Renderer/RenderInfoManager.cpp`

- ConstantBuffer生成
- `Map()`
- `Unmap()`

を検証する。バッファ名、HRESULT、フレーム番号をログに記録し、失敗時は該当描画をスキップする。

## ShadowMap.cpp

対象: `GameEngine/src/Renderer/Shadow/ShadowMap.cpp`

対象:

- サイズ0
- Device null
- Texture2D生成
- SRV生成
- DSV生成
- Begin/End時のリソース状態

影のみ無効化して通常描画を継続できる場合、Fatalにはしない。

## ShadowRenderer.cpp

対象: `GameEngine/src/Renderer/Shadow/ShadowRenderer.cpp`

DepthStencilState生成失敗を`LOG_ERROR`で記録し、ShadowRendererだけを無効化する。

## RenderTarget.cpp

対象: `GameEngine/src/System/Render/RenderTarget.cpp`

以下のHRESULTをすべて確認する。

- `CreateTexture2D`
- `CreateRenderTargetView`
- `CreateShaderResourceView`
- DepthStencil関連生成

ログにはwidth、height、DXGI_FORMAT、HRESULT、リソース名を含める。

## SamplerList.cpp

対象: `GameEngine/src/System/Render/SamplerList.cpp`

`CreateSamplerState()`の戻り値を確認し、SamplerType、Filter、AddressModeをログに記録する。

## RasterizerStateList.cpp

対象: `GameEngine/src/System/Render/RasterizerStateList.cpp`

`CreateRasterizerState()`失敗時にassertだけでなく`LOG_ERROR`を出す。Releaseビルドではnullを返し、描画をスキップする。

## InputLayoutCache.cpp

対象: `GameEngine/src/System/Render/InputLayoutCache.cpp`

Vertex Shaderがnullの場合、Shader UUID、Shaderパス、Vertex layoutを記録し、Mesh描画をスキップする。

## Renderer.h

対象: `GameEngine/include/Renderer/Renderer.h:81-87`

```cpp
assert(m_isInitialized);
```

Releaseビルドではassertが無効になる。未初期化時は`LOG_ERROR`を出し、描画を安全にスキップできる状態にする。

---

# P2: シーン・GameObject・Component

## SceneManager.cpp

対象: `GameEngine/src/Scene/SceneManager.cpp`

### 未登録シーン `:93-99`

未登録シーン要求を記録する。

- ユーザー操作ミス: `LOG_WARN`
- 初期シーン未登録: `LOG_ERROR`

現在のシーンは維持する。

### Sceneロード `:124-132`

ロード結果を確認せずに初期化している。失敗時は現在のシーンを維持し、ファイルパスを`LOG_ERROR`に記録する。

## ObjectFactory.cpp

対象: `GameEngine/src/Scene/ObjectFactory.cpp`

### Prefab null

通常の未設定ならログ不要だが、明示的なInstantiate要求でnullの場合は`LOG_ERROR`とする。

### 生成後Component

以下を検証する。

- AddComponent結果
- Transform
- RectTransform
- ObjectManager
- Scene

失敗時はGameObject生成を中止する。

## Scene.cpp

対象: `GameEngine/src/Scene/Scene.cpp`

参照解決失敗時に以下を記録する。

- UUID
- 参照元GameObject/Component
- 対象が見つからなかったこと

任意参照は`LOG_WARN`、必須参照は`LOG_ERROR`とする。

## ObjectFinder.cpp

対象: `GameEngine/src/Scene/ObjectFinder.cpp`

検索結果がnullになること自体は正常な場合があるため、ObjectFinder自身ではログを出さず、必須オブジェクトを要求する呼び出し側でログを出す。

## Transform.cpp

対象: `GameEngine/src/Components/World/Transform/Transform.cpp`

### 親子循環 `:47-54`

以下を検出しているが、現在は通知がない。

- 自分自身を親に設定
- 子孫を親に設定

通常のエディター操作ミスは`LOG_WARN`、破損シーンデータは`LOG_ERROR`とする。

### 行列異常

以下を検証する。

- Scaleが0
- NaN
- Infinity
- 壊れたQuaternion
- `Invert()`結果
- `Decompose()`結果

異常値は既定値へフォールバックし、`LOG_WARN`または`LOG_ERROR`を出す。

## CollisionMap.cpp

対象: `GameEngine/src/Managers/CollisionMap.cpp`

### 重複登録

Component TypeIdとともに`LOG_WARN`を出す。

### CheckHit null入力

`pCol1`、`pCol2`を検証し、null時は`LOG_ERROR`を出してfalseを返す。

未登録の衝突組み合わせでfalseを返すことは正常な場合があるため、毎回ログを出す必要はない。

---

# P2: UI・コンポーネント

## Canvas.cpp

対象: `GameEngine/src/Components/UI/Canvas/Canvas.cpp`

Canvasの必須Componentを検証する。

- RectTransform
- GameObject
- ImageUI

必須Component欠落時は`LOG_ERROR`を出して、そのUIの描画・ヒットテストを無効化する。

以下は正常状態なのでログ不要。

- 非アクティブUI
- RaycastTarget無効
- ヒット対象なし

## ImageUI.cpp / TextUI.cpp

対象:

- AssetManager null
- Texture/Font未ロード
- 無効Asset Handle
- Asset取得失敗

設定済みアセットのロード失敗は`LOG_WARN`または`LOG_ERROR`、非同期ロード待ちは`LOG_DEBUG`とする。

## MeshRenderer.cpp

対象:

- AssetManager null
- MeshFilter null
- 無効Mesh handle
- Mesh本体null

初期化時の必須依存欠落は`LOG_ERROR`、ロード待ちは`LOG_DEBUG`、ロード失敗は`LOG_WARN`または`LOG_ERROR`とする。

## AudioSource.cpp / SoundManager.cpp

対象:

- AssetManager null
- AudioClip未ロード
- AudioEngine null
- AudioListener未登録

AudioListener未登録は初回だけ`LOG_WARN`を出し、毎フレームは出さない。

---

# P2: カメラ・ライト

## CameraBase.cpp / StandardCamera.cpp / TargetCamera.cpp

以下を検証する。

- 自身のTransform
- TargetCameraのターゲット
- ターゲットのTransform

ターゲット未設定は`LOG_WARN`、必須Transform欠落は`LOG_ERROR`とする。デフォルトカメラへフォールバックする。

## LightManager.cpp

LightのTransform欠落を検出し、該当ライトだけ無効化する。ライト情報とGameObjectを`LOG_ERROR`に記録する。

---

# P3: 入力

## PadInput.cpp

ゲームパッド未接続は正常状態。基本的にログ不要。

接続状態を記録する場合は、状態変化時だけ`LOG_DEBUG`または`LOG_INFO`を出す。

## InputSystem.cpp

未登録入力名に対してfalse/0を返している。Binding読み込み時に一度だけ検証し、設定ミスの場合は`LOG_WARN`を出す。毎回の問い合わせではログを出さない。

---

# ログ不要な主なケース

- 未接続ゲームパッド
- 非アクティブGameObject
- 非アクティブComponent
- UIのRaycast対象外
- 任意オブジェクトが見つからない
- 未登録の衝突組み合わせ
- Scene遷移中の追加要求
- 非同期アセットロード中
- optionalなTexture/Font未設定
- ShadowMap無効でも通常描画を続行できる場合

ただし、必須設定として宣言されている場合は`LOG_WARN`または`LOG_ERROR`に昇格する。

---

# 実装優先順位

## 第1段階: ログ基盤

1. Loggerのatomic初期化
2. `LOG_FATAL`のFlush呼び出し修正
3. ファイル出力Sink追加
4. HRESULT出力形式統一
5. Error/Fatalの重複出力防止

## 第2段階: 起動・D3D初期化

対象:

- `EntryPoint.h`
- `Application.cpp`
- `DeviceResources.cpp`
- `GameEngine.cpp`

内容:

- nullチェック
- HRESULT記録
- 例外境界
- 初期化結果の返却
- デバイスロスト復旧
- 起動不能時のFatalログ

## 第3段階: AssetLoader

対象:

- `AssetLoaders.cpp`
- `AuxFileRepository.cpp`
- `AssetManager.cpp`
- `AssetRegistry.cpp`
- `ShaderLoader.cpp`
- `ShaderReflection.cpp`

内容:

- success状態の正確な設定
- 例外捕捉
- JSON検証
- Failed状態管理
- UUID/path/HRESULTのログ
- 旧アセット維持またはエラーアセットへのフォールバック

## 第4段階: 保存・読み込み

対象:

- `ObjectLoader.cpp`
- `ObjectSaver.cpp`
- `SceneEditor.cpp`
- `SceneManager.cpp`

内容:

- Save/Load APIのResult化
- JSON例外捕捉
- 必須キー検証
- null検証
- 保存失敗時の編集内容維持
- シーン切り替え失敗時の現在シーン維持

## 第5段階: GPUリソース

対象:

- `RenderTarget.cpp`
- `RenderInfoManager.cpp`
- `ShadowMap.cpp`
- `ShadowRenderer.cpp`
- `SamplerList.cpp`
- `RasterizerStateList.cpp`
- `InputLayoutCache.cpp`

内容:

- HRESULT確認
- Resource生成失敗のログ
- 該当Rendererのみ無効化
- optional機能のフォールバック
- 必須リソース失敗時のみFatal

## 第6段階: GameObject/Component境界

対象:

- `ObjectFactory.cpp`
- `Transform.cpp`
- `Scene.cpp`
- `CollisionMap.cpp`
- `CameraBase.cpp`
- `LightManager.cpp`
- UI各Component

内容:

- 必須Component検証
- null入力検証
- 未解決参照の記録
- 親子循環の警告
- Component単位の無効化
- 毎フレームログ抑制

---

# 基本方針

```text
必須初期化失敗       -> LOG_FATAL / アプリ終了
GPUデバイス失敗      -> LOG_ERROR / 復旧または終了
アセット失敗         -> LOG_ERROR / 該当アセット無効化
シーンロード失敗     -> LOG_ERROR / 現在シーン維持
描画リソース失敗     -> LOG_ERROR / 該当パスのみ停止
任意参照の欠落       -> LOG_WARN / nullまたは既定値
通常の未成立状態     -> ログ不要
```

ログを追加するだけでは復旧できない処理については、最終的に以下のAPIをboolまたはResult型へ変更することを推奨する。

- `Initialize()`
- `Load...()`
- `Save...()`
- `Create...()`
- `Replace()`
- `RequestSceneChange()`
