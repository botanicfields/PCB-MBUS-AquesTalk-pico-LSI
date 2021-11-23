# PCB-MBUS-AquesTalk-pico-LSI
# M5Stack用 AquesTalk pico LSIモジュール基板
MBUS Module of AquesTalk pico LSI for M5Stack

# 1. 概要
　「M5Stack用 AquesTalk pico LSIモジュール基板」は、市販の音声合成専用LSI「AquesTalk pico LSI」（別売）をM-BUSモジュールとしてM5Stackに取り付けるための半完成基板です。
- [スイッチサイエンス: M5Stack用 AquesTalk pico LSI モジュール基板](https://www.switch-science.com/xxxx)

　AquesTalk pico LSIの詳細は、データシートを参照ください。
- [Data Sheet: 音声合成 LSI 「AquesTalk pico LSI」ATP3011](https://www.a-quest.com/archive/manual/atp3011_datasheet.pdf)
- [Data Sheet: 音声合成 LSI 「AquesTalk pico LSI」ATP3012](https://www.a-quest.com/archive/manual/atp3012_datasheet.pdf)

### 動作例
- [Youtube: M-Bus Module of AquesTalk pico LSI for M5Stack](https://youtu.be/YfKwWRxrInM)

### 組み立て例
　組み立てにはハンダ付けが必要です。

左側: ATP3011, 右側: ATP3012  
<img src="./image/module1.JPG" width=300> <img src="./image/module2.JPG" width=300>  
写真右: M5Stack 取り付け例  
<img src="./image/module3.JPG" width=300> <img src="./image/module4.JPG" width=300>

## 1.1 特徴
- AquesTalk pico LSI（28 ピン DIP タイプ）1個を搭載できます。
- AquesTalk pico LSIのATP3011, ATP3012の両方に対応しています。
- パワーアンプ（LM4871）を内蔵し、スピーカーを直接駆動できます。
- AquestTalk pico LSIを3.3Vで動作させ、M-BUSに直結できます。
- プロトモジュール（別売）のモールドを流用し、M5Stackに取り付けできます。
- スピーカー以外のケーブル接続が不要です。
- DIP スイッチで動作モード・通信モードを設定できます。
- インタフェースをI2C, UART, SPIから選べます。
- AquesTalk pico LSIの音声出力をM-BUSから取り込めます。
- M5StackのリセットでAquesTalk pico LSIをリセットします。
- スタンドアロンモードのためのランドがあります。

DIP: Dual In-line Package  
I2C: Inte-Integrated Circuit  
UART: Universal Asynchronous Receiver Transmitter  
SPI: Serial Peripheral Interface

## 1.2 商品内容
- M-BUS モジュール基板（半完成品）1枚  
- スピーカーケーブル 1組（2ピンPH相当コネクタ付き、20cm片端ストリップ済）  
（秋月電子通商扱い）コネクタ付コード２Ｐ（Ａ）　（赤白）[DG01032-0045-01(C5679)](https://akizukidenshi.com/catalog/g/gC-05679/)
- 説明書 1部

※ AquesTalk pico LSIは付属しません。  
※ プロトモジュールは付属しません。  
※ スピーカーは付属しません

### 商品写真
写真左: 内容物、写真右: モジュール基板裏面  
<img src="./image/product1.JPG" width=300> <img src="./image/product2.JPG" width=300>

## 1.3 別途必要なもの
- AquesTalk pico LSI（秋月電子通商扱い）  
音声合成ＬＳＩ　ＡＴＰ３０１１Ｆ１－ＰＵ（ゆっくりな女性の音声）
[ATP3011F1-PU](https://akizukidenshi.com/catalog/g/gI-06220/)  
音声合成ＬＳＩ　ＡＴＰ３０１１Ｆ４－ＰＵ（かわいい女性の音声）
[ATP3011F4-PU](https://akizukidenshi.com/catalog/g/gI-05665/)  
音声合成ＬＳＩ　ＡＴＰ３０１１Ｍ６－ＰＵ（男性の音声）
[ATP3011M6-PU](https://akizukidenshi.com/catalog/g/gI-06225/)  
音声合成ＬＳＩ　ＡＴＰ３０１２Ｆ６－ＰＵ（女性の音声明瞭版）
[ATP3012F6-PU](https://akizukidenshi.com/catalog/g/gI-09973/)  
音声合成ＬＳＩ　ＡＴＰ３０１２Ｒ５－ＰＵ（小型ロボットの音声）
[ATP3012R5-PU](https://akizukidenshi.com/catalog/g/gI-11517/)
- プロトモジュール（スイッチサイエンス扱い）  
[M5Stack用プロトモジュール](https://www.switch-science.com/catalog/3650/)
- スピーカー  
4～8Ω程度、1W以上が望ましいです
- 工具  
ハンダ、ハンダごて、ニッパー、六角レンチ(0.5mm)、カッター、ピンセットなど
- M5Stack  
サンプルプログラムは、M5Stack Core Basicで動作を確認しています
- Arudino-IDEが動作する環境  
Arduino-IDE: Ver.1.18.16以降, Boards Manager: M5Stack 2.0.0以降, Library: M5Stack 3.9.0以降

# 2. モジュール基板の組み立て

## 2.1 AquesTalk pico LSI の取り付け
　別売の AquestTalk pico LSI（28 ピン DIP タイプ）1個を取り付けます。ATP3011とATP3012とでは挿入する穴が異なります。基板上のシルク印刷に従ってピンを挿入しハンダ付けします。モジュール基板裏面から飛び出しているピンを短く切ってください。

左: ATP3011, 右: ATP3012  
<img src="./image/pcb1.JPG" width=300> <img src="./image/pcb2.JPG" width=300> <img src="./image/pcb3.JPG" width=300> 

## 2.2 モールドの取り付け
　プロトモジュールのモールドおよびネジ4本を流用して、M5StackのM-BUSモジュールに仕立てることができます。モールドをモジュール基板に取り付ける前に、ボリュームやスピーカー出力コネクタの部分をモジュールから切り取ります。

<img src="./image/proto1.JPG" width=300> <img src="./image/proto2.JPG" width=300> <img src="./image/proto3.JPG" width=300> <img src="./image/proto4.JPG" width=300> <img src="./image/proto5.JPG" width=300> <img src="./image/proto6.JPG" width=300>

### ICソケットは使用できません
　ICソケットを使用すると、プロトモジュールのモールドには収まらなくなります。収まらなくなることを承知の上で1列タイプのICソケットを使用するとATP3011とATP3012を差し換えることができます。モジュール基板裏面からのピンの飛び出しの対処も大変になります。

- ICソケット（秋月電子通商扱い）  
丸ピンＩＣ用ソケット　（シングル１４Ｐ）　１×１４
[6604S-14](https://akizukidenshi.com/catalog/g/gP-03470/)  
丸ピンＩＣ用ソケット　（シングル１４Ｐ）　１×１４
[JS-11791SFTD-14](https://akizukidenshi.com/catalog/g/gP-11451/)  ※在庫限り  

<img src="./image/socket1.JPG" width=300> <img src="./image/socket2.JPG" width=300>

### その他の端子について
　J2は、ICSPです。コネクタ等は実装していません。使用にはピンヘッダをハンダ付けするなどの処置が必要です。ICSPのVCCはAquesTalk pico LSIの電源端子に繋がっているほか、M-BUSの3.3Vにも繋がっています。ICSPは、モジュール基板をM5Stackから切り離し、単体にして接続してください。  
　J3は、スタンドアロンモードで使用するPC0-3およびGNDの端子です。コネクタ等は実装していません。使用には信号ケーブルを直接ハンダ付けするなどの処置が必要です。

- ICSP: In Circuit Serial Programming

### スタンドアロン接続例
　「M5Stack用 AquesTalk pico LSIモジュール」を「M-Busエクステンション基板」に載せ、「ダイオードマトリクススイッチ基板15」を接続しています。M5Stack の電源とリセットを使用しています。
- [スイッチサイエンス: M-Busエクステンション基板キット](https://www.switch-science.com/catalog/7371/)
- [スイッチサイエンス: ダイオードマトリクススイッチ16基板](https://www.switch-science.com/catalog/xxxx/)

<img src="./image/socket3.JPG" width=300>

# 3. モジュールの設定

## 3.1 DIPスイッチによる動作モード・通信モードの設定
　動作モードおよび使用するインタフェース（通信モード）に合わせて、モジュール基板上のDIPスイッチを設定します。設定にはピンセットなどが必要です。おすすめはI2C接続です。出荷時の設定は、セーフモード・I2C 接続です。DIPスイッチの設定はリセット時に反映されます。AquesTalk pico LSIのリセットはM5Stackのリセットに繋がっています。

| 1 | 2 | 3 | 4 |動作モード|通信モード|備考|
|:-:|:-:|:-:|:-:|:-:|:-:|:- |
|OFF|OFF|OFF|ON | コマンド入力モード |I2C |I2CアドレスはEEPROMに設定した値（初期値0x2E）|
|OFF|ON |OFF|ON | セーフモード |I2C |I2Cアドレスは強制的に0x2E|
|OFF|OFF|OFF|OFF| コマンド入力モード |UART|設定されたスピードで動作 (*1) |
|OFF|ON |OFF|OFF| セーフモード |UART|強制的に9600bpsで動作|
|OFF| - |ON |OFF| - | SPI mode 3 | - |
|OFF| - |ON |ON | - | SPI mode 0 |サンプルプログラムはSPI mode 0で動作|
|ON |OFF| - | - | スタンドアロンモード | - |PC0-3の信号入力によりプリセットメッセージを選択して再生|
|ON |ON | - | - | デモモード | - |プリセットメッセージを順番に自動再生|

※ 1, 2, 3, 4は、DIPスイッチの番号。DIPスイッチはON=GNDのため、ON=0, OFF=1です。  
(*1) ATP3011の場合、SLEEP解除後の'?'送信で自動設定します。ATP3012の場合EEPROMに設定します。

## 3.2 ハンダジャンパによる信号の接続
　モジュール基板裏面のジャンパJP1-JP7をハンダで短絡（クローズ）することにより、GPIOとAquesTalk pico LSIの信号ピンを接続できます。

|ジャンパ|GPIO|AquesTalk pico LSI|出荷時|
|:-:|:-:|:-|:-:|
|JP1|16|UART-TX|オープン|
|JP2|19|SPI-MISO|オープン|
|JP3|5|SPI-SS|オープン|
|JP4|13|SLEEP|オープン|
|JP5|35|Analog Out of ATP3011|オープン|
|JP6|35|Analog Out of ATP3012|オープン|
|JP7|GND|Shut Down of Power Amplifier|クローズ|

### (1) JP1, JP2, JP3: インタフェース信号
　使用するインタフェースに合わせてJP1, JP2, JP3をハンダで短絡（クローズ）します。I2Cの場合はJP1, JP2, JP3を全てオープンのままで使用できます。他の用途に影響がなければJP1, JP2, JP3を短絡（クローズ）することで全てのインタフェースを使用可能です。出荷時はオープンです。

|インタフェース|JP1|JP2|JP3|AquestTalk pico LSIピン=GPIO|
|:-:|:-:|:-:|:-:|:- |
|I2C| - | - | - |SDA=21, SCL=22|
|UART|クローズ| - | - |TX=16, RX=17|
|SPI| - |クローズ|クローズ|SCK=18, MISO=19, MOSI=23, SS = 5|

- SDA: Serial DAta
- SCL: Serial CLock
- RX: Receive
- TX: Transmit
- MISO: Master In Slave Out
- MOSI: Master Out Slave In
- SCK: Serial ClocK
- SS: Slave Select

### (2) JP4: SLEEP 信号
　AquesTalk pico LSIのSLEEPピンをGPIO13に接続できます。GPIO13=LowでAquesTalk pico LSIがスリープ状態になります。ATP3011のUART接続において9600bpsより速い速度が必要な場合、「セーフモード」ではなく「コマンド入力モード」が必要であり、速度設定のために SLEEP の接続が必要です。出荷時はオープンです。

### (3) JP5, JP6: 音声信号の取り込み
　AquesTalk pico LSIの音声出力をGPIO35経由でM5Stackに取り込むことができます。モジュール基板に載せるのがATP3011の場合はJP5を、ATP3012の場合はJP6をハンダで短絡（クローズ）します。出荷時はオープンです。

### (4) JP7: パワーアンプのシャットダウン
　JP7がクローズ（短絡）の場合、パワーアンプのシャットダウン（SD: Shut Down）信号をGNDに接続しパワーアンプを常に動作状態とします。JP7のハンダを取り除く（オープン）にすると、AquesTalk pico LSIのPLAY信号（負極性）がシャットダウン信号となり、音声再生時以外はパワーアンプをシャットダウンすることができます。この設定では音声再生の前後でノイズが発生します。出荷時はクローズ（短絡）です。

# 4. サンプルプログラム
　AquesTalk pico LSIをM5Stackに接続し、I2C(Wire), UART(Serial), SPIで動作させるプログラムです。デモの内容は同じです。Arduino-IDE環境で使用します。モジュール基板の設定を予め使用するインタフェースに合わせておく必要があります。

### BF-034_Wire フォルダ
AqeusTalk pico LSIをI2Cで動作させるサンプルプログラムです。

### BF-034_Serial フォルダ
AqeusTalk pico LSIをUARTで動作させるサンプルプログラムです。

### BF-034_Spi フォルダ
AqeusTalk pico LSIをSPIで動作させるサンプルプログラムです。

## (1) デモの内容

### setup()
- SLEEP ピンをHighにする（デフォルト: 実行する）  
　JP4をクローズ（短絡）すると、GPIO13とAquesTalk pico LSIのSLEEPピンとが接続します。GPIO13をHIGHにしてSLEEPでない状態にします。

- UARTスピードを設定（ATP3011が対象。UARTのみ。デフォルト: 実行しない）  
　AquesTalk pico LSI (ATP3011) のUARTスピードを設定します。GPIO13がAquesTalk pico LSIのSLEEPピンと接続している必要があります。

- UARTスピードを書き換え（ATP3012が対象。UARTのみ。デフォルト: 実行しない）  
　AquesTalk pico LSI (ATP3012) のUARTスピードを書き換えます。

- I2Cアドレスを書き換え（I2Cのみ。デフォルト: 実行しない）  
　AquesTalk pico LSIのI2Cアドレスを書き換えます。

- プリセットメッセージを書き込む（デフォルト: 実行しない）  
　AquesTalk pico LSIにプリセットメッセージとして、ソースコード内のpreset_msgデータを書き込みます。時間がかかります。

- EEPROMデータをダンプ（デフォルト: 実行しない）  
　AquesTalk pico LSIのEEPROMデータをシリアルモニタに表示します。時間がかかります。

- AquesTalk pico LSIのバージョンをシリアルモニタに表示

- チャイムJを発声

- チャイムKを発声

### loop()
　ボタンを押すと、ソースコードのpreset_msgから1つを発声します。

- ボタンA
　前回発声した1つ前のものを発声します。初期値は15番目です。

- ボタンB
　前回発声したものを再度発声します。初期値は1番目です。

- ボタンC
　前回発声した1つ後のものを発声します。初期値は2番目です。

# 5. ライブラリ
　AquesTalk pico LSIを使用するためのライブラリです。

## 5.1 BF_AquesTalkPico.h
　I2C, UART, SPI共通の基本クラスです。接続するインタフェースに依ってI2C(Wire), UART(Serial), SPIのいずれかの派生クラスを使用する必要があります。

### (1) virtual int Send(const char* msg) = 0
　AquesTalk pico LSIに文字列msgを送信します。送信の様子をシリアルモニタに出力します。純粋仮想関数として定義し、使用するインタフェースに合わせて派生クラスで実装します。

### (2) virtual size_t Recv(char* res, size_t res_size) = 0
　AquesTalk pico LSIから文字列を受信します。受信完了は、'>'または'*'で判断します。純粋仮想関数として定義し、使用するインタフェースに合わせて派生クラスで実装します。

### (3) int ShowRes(int res_length_to_show = 1)
　AquesTalk pico LSIからの応答をシリアルモニタに出力します。res_length_to_showに2を渡すと、応答が1文字以下の場合にシリアルモニタへの出力を抑止できます。I2C、SPIのポーリングによる応答が正常（'>', 1文字）の場合のシリアルモニタへの出力を省略できます。

### (4) int DumpEeprom()
　AquesTalk pico LSIのEEPROM (アドレス0x000-0x3FF) の値をシリアルモニタに出力します。

### (5) int WriteEeprom(int address, int data)
　AquesTalk pico LSIのEEPROMにデータを書き込みます。

### (6) int WritePresetMsg(const char* msg[], int num_of_msg)
　AquesTalk pico LSIのEEPROMにプリセットメッセージを書き込みます。書き込みの様子をシリアルモニタに出力します。

### (7) int WriteSerialSpeed(int serial_speed)
　AquesTalk pico LSI (ATP3012) のシリアル通信速度を書き換えます。

### (8) int WriteI2cAddress(int i2c_address)
　AquesTalk pico LSIのI2Cアドレスを書き換えます。

## 5.2 BF_AquesTalkPicoWire.h
　AquesTalk pico LSIとI2Cでインタフェースする派生クラスです。

### int Begin(TwoWire &wire, int i2c_address = 0x2e);
　AquesTalk pico LSIを接続するI2Cと、I2Cアドレスを指定します。

## 5.3 BF_AquesTalkPicoSerial.h
　AquesTalk pico LSIとUARTでインタフェースする派生クラスです。

### int Begin(Stream &stream);
　AquesTalk pico LSIを接続するUARTを指定します。

## 5.4 BF_AquesTalkPicoSpi.h
　AquesTalk pico LSIとSPIでインタフェースする派生クラスです。

### int Begin(SPIClass &spi, int ss);
　AquesTalk pico LSIを接続するSPIと、SSとして使用するGPIOを指定します。

## 5.5 BF_AdcWave.h
　GPIO35から取り込んだ音声データをM5StackのLCDに表示します。表示は簡易的なものです。使用するAquesTalk pico LSIの種類（ATP3011またはATP3012）に合わせて、JP5またはJP6をクローズ（短絡）しておく必要があります。

# 6. 参考
Qiita: [AquesTalk pico LSI を M5Stack の I2C, UART, SPI で動かす](https://qiita.com/BotanicFields/items/fff644f408c291e5a5f0)
