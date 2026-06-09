# M5Monitor

M5Stack Basic V2.7 向けの卓上モニターです。
カレンダー、Netwatch のネットワーク状態、AquaPi の水槽状態を 320x240 の画面に表示します。

## 機能

- カレンダー表示
  - 月進捗リング付きの日付表示
  - 平日進捗表示
  - 月間カレンダー表
- Netwatch 表示
  - compact API から現在の監視レベル、ラベル、主要理由、履歴を取得
  - `ok` / `warning` / `critical` を色で表示
- AquaPi 表示
  - compact API から水槽ごとの温度と状態を取得
  - 最大 5 水槽まで一覧表示
  - 漏水状態を `LEAK SAFE` / `LEAK ALERT!` として表示
- Auto 表示
  - Calendar / Netwatch / AquaPi を一定間隔で自動ローテーション
- Alert Mode
  - 漏水または水温 danger を検知した場合に Alert 専用画面へ固定
  - Alert 発動中は警告音を断続的に鳴らす
- 時間帯別の自動輝度調整
  - 07:00 - 18:59: `60`
  - 19:00 - 23:59: `20`
  - 00:00 - 06:59: `10`

## 必要なもの

- M5Stack Basic V2.7
- ESP32 Arduino 環境
- `M5Unified`
- `ArduinoJson`
- Wi-Fi 接続
- NTP へ到達できるネットワーク

## ファイル

- `m5monitor.ino`: 本体スケッチ
- `secrets.h`: Wi-Fi SSID とパスワードを定義するローカルファイル
- `secrets.example.h`: `secrets.h` のテンプレート
- `settings.h`: Netwatch / AquaPi の URL と更新間隔を定義する設定ファイル

`secrets.h` は `.gitignore` 対象です。実値をリポジトリに含めないでください。

## セットアップ

1. `secrets.example.h` を `secrets.h` にコピーします。
2. `secrets.h` の `WIFI_SSID` と `WIFI_PASSWORD` を設定します。
3. 必要に応じて `settings.h` の URL や間隔を変更します。
4. Arduino IDE などで `m5monitor.ino` を M5Stack Basic V2.7 に書き込みます。

## 設定

`settings.h` で次の値を設定できます。

| 定義 | 既定値 | 内容 |
| --- | --- | --- |
| `NETWATCH_COMPACT_URL` | `http://netpi:8080/api/monitoring/compact` | Netwatch compact API の URL |
| `AQUAPI_COMPACT_URL` | `http://aquapi:8080/api/monitoring/compact` | AquaPi compact API の URL |
| `AQUAPI_LEAK_LATEST_URL` | `http://aquapi:8080/api/leak/latest` | Alert Mode 中に使う漏水 latest API の URL |
| `AQUAPI_TANKS_LATEST_URL` | `http://aquapi:8080/api/tanks/latest` | Alert Mode 中に使う水槽 latest API の URL |
| `NETWATCH_FETCH_INTERVAL_SECONDS` | `30` | Netwatch の取得間隔 |
| `AQUAPI_FETCH_INTERVAL_SECONDS` | `60` | 通常時の AquaPi compact API 取得間隔 |
| `AQUAPI_ALERT_FETCH_INTERVAL_SECONDS` | `15` | Alert Mode 中の latest API 取得間隔 |
| `AUTO_CALENDAR_SECONDS` | `30` | Auto 表示で Calendar を表示する秒数 |
| `AUTO_NETWATCH_SECONDS` | `15` | Auto 表示で Netwatch を表示する秒数 |
| `AUTO_AQUAPI_SECONDS` | `15` | Auto 表示で AquaPi を表示する秒数 |
| `ALARM_BEEP_ON_MS` | `120` | 警告音 1 回の鳴動時間 |
| `ALARM_BEEP_OFF_MS` | `380` | 警告音の無音時間 |
| `ALARM_BEEP_FREQUENCY_HZ` | `2200` | 警告音の周波数 |
| `ALARM_VOLUME` | `255` | 警告音の音量 |

時刻は NTP で同期します。タイムゾーンは JST (`JST-9`) 固定です。

## 操作

| ボタン | 動作 |
| --- | --- |
| A / 左 | Calendar 表示中はページ切り替え。その他の表示では再描画 |
| B / 中央 | 表示モード切り替え。`Calendar -> Netwatch -> AquaPi -> Auto -> Calendar` |
| C / 右 | 表示中モードのデータを強制取得して再描画。Calendar 表示中は時刻同期 |

Alert Mode 中は 3 ボタンすべてが警告音停止だけに使われます。ボタンでは Alert Mode 自体を解除しません。

ボタン入力は 250ms のデバウンスがあります。

## 表示モード

### Calendar

Calendar には 3 ページあります。

- Default: 年、月、日、曜日、月の進捗リング
- Work: 平日は `DAY n/5` と平日進捗、週末は `FREE`
- Table: 月間カレンダー表と当日ハイライト

日付はゼロ埋め表示です。

### Netwatch

`NETWATCH_COMPACT_URL` から JSON を取得します。
主に次のフィールドを利用します。

- `level`
- `label`
- `alert`
- `title`
- `message`
- `issue_count`
- `primary_reason`
- `history.points[].level`

`primary_reason` は文字列、または `code` / `target` を持つオブジェクトとして扱います。
履歴は最大 24 点を小さな矩形で表示します。

### AquaPi

`AQUAPI_COMPACT_URL` から水槽状態 JSON を取得します。
主に次のフィールドを利用します。

- `level`
- `label`
- `alert`
- `title`
- `message`
- `issue_count`
- `tanks[].short_name_ascii`
- `tanks[].name`
- `tanks[].sensor_id`
- `tanks[].temperature_c`
- `tanks[].status`
- `tanks[].alert`
- `leak.status`
- `leak.alert`
- `leak.label`

Alert Mode 中は発生中のアラート種別に応じて latest API を取得します。

- 漏水アラート中: `AQUAPI_LEAK_LATEST_URL`
- 水温 danger アラート中: `AQUAPI_TANKS_LATEST_URL`
- 漏水と水温 danger が同時の場合: 両方

水槽名は `short_name_ascii`、`name`、`sensor_id` の順で利用します。
温度がない場合は `--.-` と表示します。
漏水は `leak.alert == true` または `leak.status == "wet"` のとき `LEAK ALERT!` と表示し、それ以外は `LEAK SAFE` と表示します。

### Auto

Auto では Calendar、Netwatch、AquaPi をローテーション表示します。
表示中は右上に `AUTO` バッジが出ます。

### Alert Mode

Alert Mode は通常時の AquaPi compact API の結果から内部的に発動します。
通常の表示モード選択には出ません。

Alert Mode の発動条件は次のいずれかです。

- `leak.alert == true`
- `leak.status == "wet"`
- `tanks[].status == "danger"` の水槽が 1 つ以上ある

Alert Mode 中は Alert 専用画面に固定されます。
漏水と水温 danger が同時に発生している場合は、漏水を優先して表示し、水温 danger の件数を補足表示します。

Alert Mode 発動時は警告音を開始します。
いずれかのボタンを押すと警告音だけを停止します。同じ Alert Mode 継続中は再鳴動しません。

解除条件は、安全状態を連続 3 回確認することです。
安全状態とは、漏水 alert がなく、`leak.status != "wet"` で、`status == "danger"` の水槽がない状態です。
Alert Mode 中の latest API 取得間隔が 15 秒の場合、最短 45 秒で自動解除されます。
通信エラーは安全状態として扱わず、Alert Mode を維持します。

## 更新と再試行

- 起動時に Wi-Fi 接続と NTP 同期を行います。
- 起動時に Wi-Fi 接続できなかった場合、約 10 分ごとに再試行します。
- 時計、日付、輝度は約 1 秒ごとに確認します。
- 通常時は AquaPi compact API を 60 秒間隔で定期取得します。
- Alert Mode 中は発生中のアラート種別に応じて latest API を 15 秒間隔で取得します。
- 復旧後は通常時の AquaPi compact API 監視に戻ります。
- Netwatch の取得は表示中のモードだけを対象にします。
- HTTP 取得失敗や JSON パース失敗は画面上にエラーとして表示します。
