# PicConfusion Codec by Windust
贴吧图片混淆吧的混淆算法C++实现

## Usage

picconfusion [options]...

## Options

- `-h, --help`: Display help information and exit
- `-i <path>, --input <path>`: Set the input image path
- `-o <path>, --output <path>`: Set the output image path
- `-e, --encrypt`: Encrypt operation
- `-d, --decrypt`: Decrypt operation
- `-t <type>, --type <type>`: Set the encryption/decryption type
  - `0: Block Confusion`
  - `1: Pixel Confusion`
  - `2: Row Pixel Confusion`
  - `3: PicEncrypt Row Confusion`
  - `4: PicEncrypt Row+Column Confusion`
- `-bx <size>, --blockX <size>`: Set the block size in the X direction
- `-by <size>, --blockY <size>`: Set the block size in the Y direction
- `-k <key>, --key <key>`: Set the encryption/decryption key

## Examples

### Encrypt image

picconfusion -i input.png -o output.png -e -t 0 -bx 50 -by 50 -k 0.666

### Decrypt image

picconfusion -i input.png -o output.png -d -t 0 -bx 50 -by 50 -k 0.666

## Note

1. For Block Confusion type (TYPE_BC), the block size (`-bx`, `-by`) must be greater than 0.
2. For other types, the block size arguments will be ignored.
3. The key (`-k`) is a string that may require a specific format depending on the encryption/decryption type.
4. The input image (`-i`) must be a valid image file path.
5. The output image (`-o`) can be any path where you want to save the encrypted/decrypted result.
