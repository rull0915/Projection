import os
import tkinter as tk
from tkinter import filedialog, messagebox

# ファイルをC++のヘッダーファイルに変換する関数
# input_path: 変換する元のファイルパス
# output_path: 生成するヘッダーファイルのパス
# variable_name: ヘッダーファイル内で使用する変数名
def file_to_cpp_header(input_path, variable_name):
    # 入力ファイルの存在を確認
    if not os.path.exists(input_path):
        # 存在しない場合はエラーメッセージを表示して終了
        messagebox.showerror("エラー", f"{input_path} が見つかりません。")
        return

    # 変数名が空でないことを確認
    if not variable_name.strip():
        messagebox.showerror("エラー", "変数名を指定してください。")
        return

    # 出力ファイルのパスを自動生成する
    dir_name = os.path.dirname(input_path)
    base_name = os.path.splitext(os.path.basename(input_path))[0]
    output_path = os.path.join(dir_name, f"{base_name}.h")

    # 変換処理を実行
    try:
        # 読み込み/バイナリとしてファイルを読み込む
        with open(input_path, mode="rb") as f:
            # ファイルの内容をバイト列として読み込む
            data = f.read()

        # C++コードを構築
        lines = []
        # 自動生成されたことを示すコメントを追加 : ファイル名を取得し追記
        lines.append(f"// Automatically generated from {os.path.basename(input_path)}")
        # インクルードガードを追加
        lines.append("#pragma once\n")
        # 名前空間を追加
        lines.append("namespace REngine::AssetData {")
        # バイナリデータを格納する変数宣言 : 指定された名前で変数を作成
        lines.append(f"    inline const unsigned char {variable_name}_data[] = {{")

        # 1行あたり16バイトずつ16進数で書き出す
        for i in range(0, len(data), 16):
            # 16バイトごとにデータを分割
            chunk = data[i:i+16]
            # バイト列を16進数表記に変換し、カンマで区切る
            hex_line = ", ".join(f"0x{b:02X}" for b in chunk)
            # 最後の要素でなければカンマを付ける
            if i + 16 < len(data):
                hex_line += ","
            # 構築した16進数を追加
            lines.append("        " + hex_line)

        # バイト列の終わりを示す閉じ括弧を追加
        lines.append("    };")
        # バイト列のサイズを示す変数を追加
        lines.append(f"    inline const unsigned int {variable_name}_size = sizeof({variable_name}_data);")
        # 名前空間の閉じ括弧を追加
        lines.append("} // namespace REngine::AssetData")
        # ヘッダーファイルとして書き出し
        with open(output_path, "w", encoding="utf-8") as f:
            f.write("\n".join(lines))

        messagebox.showinfo("成功", f"Success: {output_path} を生成しました。 ({len(data)} bytes)")
        
    except Exception as e:
        # 例外が発生した場合はエラーメッセージを表示
        messagebox.showerror("エラー", f"ファイルの変換中にエラーが発生しました: {str(e)}")

# 使い方例
# file_to_cpp_header("folder.png", "g_FolderIcon")

# --- GUI（画面）の構築 ---
def select_file():
    # 拡張子はすべてのファイルに対応
    file_path = filedialog.askopenfilename(
        title="アセットファイルを選択",
        filetypes=[("All Files", "*.*")]
    )

    # 選択されたファイルパスが空でない場合、エントリに表示
    if file_path:
        entry_input.delete(0, tk.END)
        entry_input.insert(0, file_path)
        
        # ファイル名から自動で変数名の初期候補を入力（例: folder.png -> g_folder）
        base_name = os.path.splitext(os.path.basename(file_path))[0]
        entry_var.delete(0, tk.END)
        entry_var.insert(0, f"g_{base_name}")

def on_generate():
    file_to_cpp_header(entry_input.get(), entry_var.get())

# メインウィンドウの設定
root = tk.Tk()                      # Tkinterのメインウィンドウを作成
root.title("Engine Asset Embedder") # ウィンドウタイトルを設定
root.geometry("500x220")            # ウィンドウサイズを設定
root.resizable(False, False)        # ウィンドウのサイズ変更を禁止

# UI要素の配置
# 入力ファイル選択のラベルとエントリ、参照ボタンを配置
tk.Label(root, text="Select File:", anchor="w").pack(fill="x", padx=15, pady=(15, 2))
frame_input = tk.Frame(root)            # 入力ファイル選択用のフレームを作成
frame_input.pack(fill="x", padx=15)     # フレームをウィンドウに配置

entry_input = tk.Entry(frame_input)     # 入力ファイルパスを表示するエントリを作成
entry_input.pack(side="left", fill="x", expand=True, padx=(0, 5))   # エントリをフレームの左側に配置し、横方向に拡張可能に設定

btn_browse = tk.Button(frame_input, text="参照...", command=select_file)    # 参照ボタンを作成し、クリック時にselect_file関数を呼び出す
btn_browse.pack(side="right")           # 参照ボタンをフレームの右側に配置

tk.Label(root, text="Variable Name (C++):", anchor="w").pack(fill="x", padx=15, pady=(15, 2))

entry_var = tk.Entry(root)
entry_var.pack(fill="x", padx=15)

# 生成ボタンを配置  
btn_generate = tk.Button(root, text="Generate C++ Header", bg="#4CAF50", fg="white", font=("Arial", 11, "bold"), command=on_generate)
btn_generate.pack(fill="x", padx=15, pady=25)   # 生成ボタンをウィンドウに配置し、クリック時にon_generate関数を呼び出す

root.mainloop()