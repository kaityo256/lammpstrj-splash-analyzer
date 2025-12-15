# lammpstrj-splash-analyzer

## 概要

lammpstrjファイルを解析して、クラスター数を出力する。

## 使い方

まず、リポジトリをクローンする。

```sh
cd github
git clone --recursive https://github.com/kaityo256/lammpstrj-splash-analyzer.git
```

その後、ディレクトリの中で`make`する。

```sh
cd lammpstrj-splash-analyzer
make
```

すると、`splashcount`というファイルができるので、それをdroplet.lammpstrjのあるところで実行する。

```sh
~/github/lammpstrj-splash-analyzer/splashcount impact.lammpstrj
```

すると、そこに`droplet_count.dat`ができる。これが、フレームごとのクラスター数となる。

## ライセンス

MIT
