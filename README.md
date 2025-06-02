# 100base-t1-converter for Education
学習用 100Base-T1 MediaConnverter

### 特徴
- NXP社のTJA1101とCoreChips社のSR8201F（Realtek社 RTL8201F互換）を使用した 100Base-T1 <-> 100Base-TX のMediaConverterです。
- STM32F103C8T6を搭載したBlue Pill[参考URL](https://en.wikipedia.org/wiki/STM32)を使うことでPHYの設定をMDIOを用いて確認／実施することができ、PHY層の動きを学習することができます。
- オリジナルは Creative Commons Attribution-ShareAlike 4.0 International ライセンスで公開されている[100base-t1-converter](https://github.com/ehntoo/100base-t1-converter)です。作者はehntoo様です。
- 本プロダクトはベースプロダクトから、MDIOでPHYの設定を読み書きするプログラムを追加し、また、[LCSC](https://www.lcsc.com/)で部品を注文するための具体的な部品番号の追加および部品配置の変更と、基板を[JLCPCB](https://jlcpcb.com/)でより安く注文できるように最小ドリル径を変更したものとなります。
- 本プロダクトのライセンスは Creative Commons Attribution-ShareAlike 4.0 International を継承します。

### 基板
![TopImage](/docs/100base-t1-converter_Top_Image.png)
![BottomImage](/docs/100base-t1-converter_Bottom_Image.png)
![TopPhoto](/docs/100base-t1-converter_Top_Photo.jpg)
![BottomPhoto](/docs/100base-t1-converter_Bottom_Photo.jpg)
（※基盤イメージ／写真は参考であり、改良のため変更することがあります）

### 基板の設定
![TopImageExplanation](/docs/100base-t1-converter_Top_Image_explanation.jpg)

### 使用例
- メディアコンバータとして利用するだけならば、下記ジャンパの設定で、Blue Pillとそれに書き込むソフトウェアの対応は不要です。

![UseCase_01](/docs/UseCase_01.jpg)

- Blue Pillにソフトウェアを書き込むことで、MDIOを通してPYHチップの動作確認、変更が可能です。

[レジスター読み書きソフト](/software/phyRegRead/)

- Blue PillをArduinoとして利用し、上記ソフトを書き込むことで、レジスターの読み書きができます。

![UseCase_03](/docs/UseCase_02.jpg)
（ch1がPB7のMDC(Clock)、ch2がPB6のMDIO(Data)です）

![UseCase_04](/docs/UseCase_03.png)

- PHYADDR：TJA1101=0x06, SR8201F=0x02
- シリアルコマンド：a 送信=全レジスタ読み出し, r (phyaddr) (register number) 送信=指定レジスタ読出, w (phyaddr) (register number) (レジスタ値) 送信=指定レジスタ書込


### 販売ページ

- スイッチサイエンス様

### 資料

- [サポートページ](https://github.com/TLDSJPWORK/100base-t1-converter)
- [回路図](docs/100base-t1-converter.pdf)
- [TJA1101データシート](https://www.nxp.com/docs/en/data-sheet/TJA1101B.pdf)
- [SR8201Fデータシート](https://www.lcsc.com/datasheet/lcsc_datasheet_2409272302_CoreChips-SR8201F_C378491.pdf) 
- [Blue Pill](https://en.wikipedia.org/wiki/STM32)
- [STM32F103 ボードを Arduino ライクに使う](https://ht-deko.com/arduino/stm32f103c8t6.html)
- [MDIOとは | Ethernet(イーサネット)技術コラム](https://www.e-nexty.com/static/articles/column_ethernet_mdio)
