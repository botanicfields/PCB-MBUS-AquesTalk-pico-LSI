# PCB-MBUS-AquesTalk-pico-LSI
# M5Stack用 AquesTalk pico LSIモジュール基板
MBUS Module of AquesTalk pico LSI for M5Stack

# 1. 概要
　市販の音声合成専用LSI「AquesTalk pico LSI」（別売）をM-BUSモジュールとしてM5Stackに取り付けるための半完成基板です。  
　AquesTalk pico LSIの詳細は、データシートを参照ください。
- [Data Sheet: 音声合成 LSI 「AquesTalk pico LSI」ATP3011](https://www.a-quest.com/archive/manual/atp3011_datasheet.pdf)
- [Data Sheet: 音声合成 LSI 「AquesTalk pico LSI」ATP3012](https://www.a-quest.com/archive/manual/atp3012_datasheet.pdf)

### 委託販売
（スイッチサイエンス扱い）[M5Stack用 AquesTalk pico LSI モジュール基板](https://www.switch-science.com/catalog/7624/)

### 動作例
（YouTube）[M-Bus Module of AquesTalk pico LSI for M5Stack](https://youtu.be/YfKwWRxrInM)

### 組み立て例
　組み立てにはハンダ付けが必要です。

左側: ATP3011, 右側: ATP3012  
<img src="./image/module1.JPG" width=300> <img src="./image/module2.JPG" width=300>  
写真右: M5Stack 取り付け例  
<img src="./image/module3.JPG" width=300> <img src="./image/module4.JPG" width=300>

## 1.1 特徴
- AquesTalk pico LSI（28ピンDIPタイプ）1個を搭載できます。
- ATP3011, ATP3012の両方に対応しています。
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
（秋月電子通商扱い）[コネクタ付コード２Ｐ（Ａ）（赤白）DG01032-0045-01(C5679)](https://akizukidenshi.com/catalog/g/gC-05679/)
- 説明書 1部

※ AquesTalk pico LSIは付属しません。  
※ プロトモジュールは付属しません。  
※ スピーカーは付属しません。

### 商品写真
写真左: 内容物、写真右: モジュール基板裏面  
<img src="./image/product1.JPG" width=300> <img src="./image/product2.JPG" width=300>

## 1.3 別途必要なもの
- AquesTalk pico LSI（秋月電子通商扱い）  
[音声合成ＬＳＩ　ＡＴＰ３０１１Ｆ１－ＰＵ（ゆっくりな女性の音声）
ATP3011F1-PU](https://akizukidenshi.com/catalog/g/gI-06220/)  
[音声合成ＬＳＩ　ＡＴＰ３０１１Ｆ４－ＰＵ（かわいい女性の音声）
ATP3011F4-PU](https://akizukidenshi.com/catalog/g/gI-05665/)  
[音声合成ＬＳＩ　ＡＴＰ３０１１Ｍ６－ＰＵ（男性の音声）
ATP3011M6-PU](https://akizukidenshi.com/catalog/g/gI-06225/)  
[音声合成ＬＳＩ　ＡＴＰ３０１２Ｆ６－ＰＵ（女性の音声明瞭版）
ATP3012F6-PU](https://akizukidenshi.com/catalog/g/gI-09973/)  
[音声合成ＬＳＩ　ＡＴＰ３０１２Ｒ５－ＰＵ（小型ロボットの音声）
ATP3012R5-PU](https://akizukidenshi.com/catalog/g/gI-11517/)
- プロトモジュール  
（スイッチサイエンス扱い）[M5Stack用プロトモジュール](https://www.switch-science.com/catalog/3650/)
- スピーカー  
4～8Ω程度、1W以上が望ましいです。
- 工具  
ハンダ、ハンダごて、ニッパー、六角レンチ(0.5mm)、カッター、ピンセットなど
- M5Stack  
サンプルプログラムは、M5Stack Core Basicで動作を確認しています。
- Arudino-IDEが動作する環境  

# 2. モジュール基板の組み立て

## 2.1 AquesTalk pico LSI の取り付け
　別売のAquestTalk pico LSI（28ピンDIPタイプ）1個を取り付けます。ATP3011とATP3012とでは挿入する穴が異なります。基板上のシルク印刷に従ってピンを挿入しハンダ付けします。モジュール基板裏面から飛び出しているピンを短く切ってください。

左: ATP3011, 右: ATP3012  
<img src="./image/pcb1.JPG" width=300> <img src="./image/pcb2.JPG" width=300> <img src="./image/pcb3.JPG" width=300> 

## 2.2 モールドの取り付け
　プロトモジュール（別売）のモールドおよびネジ4本を流用して、M5StackのM-BUSモジュールに仕立てることができます。モールドを基板に取り付ける前に、ボリュームやスピーカー出力コネクタの部分をモールドから切り取ります。

<img src="./image/proto1.JPG" width=300> <img src="./image/proto2.JPG" width=300> <img src="./image/proto3.JPG" width=300> <img src="./image/proto4.JPG" width=300> <img src="./image/proto5.JPG" width=300> <img src="./image/proto6.JPG" width=300>

## 2.3 スピーカーの接続
　付属のケーブルでJ4とスピーカー（別売）を接続します。GNDには接続しないでください。

### ICソケットは使用できません
　ICソケットを使用すると、プロトモジュールのモールドには収まらなくなります。収まらなくなることを承知の上で1列タイプのICソケットを使用するとATP3011とATP3012を差し換えることができます。モジュール基板裏面からのピンの飛び出しの対処も大変になります。

- ICソケット（秋月電子通商扱い）  
[丸ピンＩＣ用ソケット　（シングル１４Ｐ）　１×１４　
6604S-14](https://akizukidenshi.com/catalog/g/gP-03470/)  
[丸ピンＩＣ用ソケット　（シングル１４Ｐ）　１×１４　
JS-11791SFTD-14](https://akizukidenshi.com/catalog/g/gP-11451/)  ※在庫限り  

<img src="./image/socket1.JPG" width=300> <img src="./image/socket2.JPG" width=300>

### その他の端子について
　J2は、ICSPです。コネクタ等は実装していません。使用にはピンヘッダをハンダ付けするなどの処置が必要です。ICSPのVCCはAquesTalk pico LSIの電源端子に繋がっているほか、M-BUSの3.3Vにも繋がっています。ICSPは、モジュール基板をM5Stackから切り離し、単体にして接続してください。  
　J3は、スタンドアロンモードで使用するPC0-3およびGNDの端子です。コネクタ等は実装していません。使用には信号ケーブルを直接ハンダ付けするなどの処置が必要です。

- ICSP: In Circuit Serial Programming

### スタンドアロン接続例
　M5Stack用 AquesTalk pico LSIモジュールをM-Busエクステンションに載せ、ダイオードマトリクススイッチ16基板を接続しています。M5Stackの電源とリセットを使用しています。  
（スイッチサイエンス扱い）[M-Busエクステンション基板キット](https://www.switch-science.com/catalog/7371/)  
（スイッチサイエンス扱い）[ダイオードマトリクススイッチ16基板](https://www.switch-science.com/catalog/7626/)  
<img src="./image/socket3.JPG" width=300>

# 3. モジュールの設定

## 3.1 DIPスイッチによる動作モード・通信モードの設定
　動作モードおよび使用するインタフェース（通信モード）に合わせて、モジュール基板上のDIPスイッチを設定します。設定にはピンセットなどが必要です。おすすめはI2C接続です。出荷時の設定は、セーフモード・I2C接続です。DIPスイッチの設定はリセット時に反映されます。AquesTalk pico LSIのリセットはM5Stackのリセットに繋がっています。

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
　AquesTalk pico LSIのSLEEPピンをGPIO13に接続できます。GPIO13=LowでAquesTalk pico LSIがスリープ状態になります。ATP3011のUART接続において9600bpsより速い速度が必要な場合、「セーフモード」ではなく「コマンド入力モード」に設定し、速度設定のためにSLEEPを使用します。出荷時はオープンです。

### (3) JP5, JP6: 音声信号の取り込み
　AquesTalk pico LSIの音声出力をGPIO35経由でM5Stackに取り込むことができます。ATP3011の場合はJP5を、ATP3012の場合はJP6をハンダで短絡（クローズ）します。出荷時はオープンです。

### (4) JP7: パワーアンプのシャットダウン
　JP7がクローズ（短絡）の場合、パワーアンプのシャットダウン（SD: Shut Down）信号をGNDに接続しパワーアンプを常に動作状態とします。JP7のハンダを取り除く（オープン）にすると、AquesTalk pico LSIのPLAY信号（負極性）がシャットダウン信号となり、パワーアンプの消費電力を減らすことができます。この設定では音声再生の前後でノイズが発生します。出荷時はクローズ（短絡）です。

# 4. サンプルプログラム
　AquesTalk pico LSIをM5Stackに接続し、I2C(Wire), UART(Serial), SPIで動作させるプログラムです。デモの内容は同じです。Arduino-IDE環境で使用します。モジュール基板の設定を予め使用するインタフェースに合わせておく必要があります。

### BF-034_Wireフォルダ
　AqeusTalk pico LSIをI2Cで動作させるサンプルプログラムです。

### BF-034_Serialフォルダ
　AqeusTalk pico LSIをUARTで動作させるサンプルプログラムです。

### BF-034_Spiフォルダ
　AqeusTalk pico LSIをSPIで動作させるサンプルプログラムです。

### BF-034_Core2_Wireフォルダ
　Core2にモジュールを装着して、AqeusTalk pico LSIをI2Cで動作させるサンプルプログラムです。M5Stack Core1との違いは以下です

- includeするヘッダーファイル
- WireのGPIOピン番号、SleepのGPIOピン番号

## 4.1 ボタン操作 .. loop()
　ボタンを押すと、ソースコードのpreset_msgを発声します。初期値はpreset_msg[0]です。

| M5 | ボタン | 停止中 | 発声中 |
|:-:|:-:|:-:|:-:|
|M5Stack| A   | 1つ前のpreset_msgを発声 | 発声を中断して1つ前のpreset_msgを発声 |
|M5Stack| B   | 現在のpreset_msgを発声 | 発声を中断して停止 |
|M5Stack| C   | 1つ後のpreset_msgから連続して発声 | 発声を中断して1つ後のpreset_msgから連続して発生 |

## 4.2 setup()
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

# 5. ライブラリ
　AquesTalk pico LSIを使用するためのライブラリです。

## 5.1 BF_AquesTalkPico.h
　I2C, UART, SPI共通の基本クラスです。接続するインタフェースに依ってI2C(Wire), UART(Serial), SPIのいずれかの派生クラスを使用する必要があります。

### (1) virtual int Send(const char* msg) = 0
　AquesTalk pico LSIに文字列msgを送信します。送信の様子をシリアルモニタに出力します。純粋仮想関数として定義し、使用するインタフェースに合わせて派生クラスで実装します。

### (2) virtual size_t Recv(char* res, size_t res_size) = 0
　AquesTalk pico LSIから文字列を受信します。受信完了は、'>'または'*'で判断します。純粋仮想関数として定義し、使用するインタフェースに合わせて派生クラスで実装します。

### (3) virtual bool Busy() = 0
　発声中などAquesTalk pico LSIが処理中の場合trueを応答します。falseの場合発声が終了したと判断できます。純粋仮想関数として定義し、使用するインタフェースに合わせて派生クラスで実装します。I2C, SPIではおのずとポーリングができますが、UARTではSend()による明示的なポーリングが必要です。

### (4) int ShowRes(int res_length_to_show = 1)
　AquesTalk pico LSIからの応答をシリアルモニタに出力します。res_length_to_showに2を渡すと、応答が1文字以下の場合にシリアルモニタへの出力を抑止できます。I2C、SPIのポーリングによる応答が正常（'>', 1文字）の場合のシリアルモニタへの出力を省略できます。

### (5) int DumpEeprom()
　AquesTalk pico LSIのEEPROM (アドレス0x000-0x3FF) の値をシリアルモニタに出力します。

### (6) int WriteEeprom(int address, int data)
　AquesTalk pico LSIのEEPROMにデータを書き込みます。

### (7) int WritePresetMsg(const char* msg[], int num_of_msg)
　AquesTalk pico LSIのEEPROMにプリセットメッセージを書き込みます。書き込みの様子をシリアルモニタに出力します。

### (8) int WriteSerialSpeed(int serial_speed)
　AquesTalk pico LSI (ATP3012) のシリアル通信速度を書き換えます。

### (9) int WriteI2cAddress(int i2c_address)
　AquesTalk pico LSIのI2Cアドレスを書き換えます。

## 5.2 BF_AquesTalkPicoWire.h
　AquesTalk pico LSIとI2Cでインタフェースする派生クラスです。

### int Begin(TwoWire &wire, int i2c_address = 0x2e);
　AquesTalk pico LSIを接続するI2CとI2Cアドレスを指定します。

## 5.3 BF_AquesTalkPicoSerial.h
　AquesTalk pico LSIとUARTでインタフェースする派生クラスです。

### int Begin(Stream &stream);
　AquesTalk pico LSIを接続するUARTを指定します。

## 5.4 BF_AquesTalkPicoSpi.h
　AquesTalk pico LSIとSPIでインタフェースする派生クラスです。

### int Begin(SPIClass &spi, int ss);
　AquesTalk pico LSIを接続するSPIとSSとして使用するGPIOを指定します。

# 6. BF-034_Clockフォルダ
　M5StackのLCDに日付・時刻を表示する置時計のデモです。

- リセット後はsetupの状況を表示します。setupが完了しボタンCを押すと時計表示になります。
- ボタンBで日付を音声で読み上げます。ボタンCで時刻を音声で読み上げます。

### ご注意
　WiFi動作中に発声すると電圧低下を検出してリブートすることがあります。

### 参考情報

- Qiita: [ESP32のWiFiおよびtzapu/WiFiManagerにおいて回復処理を試行錯誤した](https://qiita.com/BotanicFields/items/8a73101a8bfe51e57f67)
- Qiita: [ESP32 において NTP の時刻同期を捕まえて RTC を更新する](https://qiita.com/BotanicFields/items/f1e28af5a63e4ccf7023)
- Qiita: [M5Atom, M5Stack Core 用の I2C リアルタイムクロック基板を作って動かす](https://qiita.com/BotanicFields/items/dc35e12423be8f6e9b4e)

# 7. 参考
- Qiita: [AquesTalk pico LSI を M5Stack の I2C, UART, SPI で動かす](https://qiita.com/BotanicFields/items/fff644f408c291e5a5f0)
