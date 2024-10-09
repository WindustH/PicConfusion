# PicConfusion Codec by Windust

贴吧图片混淆吧的混淆算法C++实现

## 关于错误的 PNG 文件的处理
从以往的各种图片混淆网站保存的 PNG，文件尾普遍丢失两个字节，该程序补齐最后四个字节为 AE 42 60 82

# 文档

## Usage

PicConfusion [options]...

## Options

- `-h, --help`
  - Display help information and exit

- `-i <path>, --input <path>`
  - Set the input image path

- `-o <path>, --output <path>`
  - Set the output image path

- `-e, --encrypt`
  - Encrypt image

- `-d, --decrypt`
  - Decrypt image

- `-t <type>, --type <type>`
  - Set the encryption/decryption type
    - `0: Block Confusion (MD5 Shuffle)`
    - `1: Pixel Confusion (MD5 Shuffle)`
    - `2: Row Pixel Confusion (MD5 Shuffle)`
    - `3: PicEncrypt Row Confusion (Logistic Map)`
    - `4: PicEncrypt Row+Column Confusion (Logistic Map)`
    - `5: Tomato Confusion (Hilbert Space-Filling Curve)`

- `-bx <size>, --blockX <size>`
  - Set the block size in the X direction
    - Required by:
      - `0: Block Confusion (MD5 Shuffle)`

- `-by <size>, --blockY <size>`
  - Set the block size in the Y direction
    - Required by:
      - `0: Block Confusion (MD5 Shuffle)`

- `-k <key>, --key <key>`
  - Set the encryption/decryption key
    - Required by:
      - `0: Block Confusion (MD5 Shuffle)`
      - `1: Pixel Confusion (MD5 Shuffle)`
      - `2: Row Pixel Confusion (MD5 Shuffle)`
      - `3: PicEncrypt Row Confusion (Logistic Map)`
      - `4: PicEncrypt Row+Column Confusion (Logistic Map)`
