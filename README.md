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
- Auto 表示
  - Calendar / Netwatch / AquaPi を一定間隔で自動ローテーション
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
| `NETWATCH_FETCH_INTERVAL_SECONDS` | `30` | Netwatch の取得間隔 |
| `AQUAPI_FETCH_INTERVAL_SECONDS` | `60` | AquaPi の取得間隔 |
| `AUTO_CALENDAR_SECONDS` | `30` | Auto 表示で Calendar を表示する秒数 |
| `AUTO_NETWATCH_SECONDS` | `15` | Auto 表示で Netwatch を表示する秒数 |
| `AUTO_AQUAPI_SECONDS` | `15` | Auto 表示で AquaPi を表示する秒数 |

時刻は NTP で同期します。タイムゾーンは JST (`JST-9`) 固定です。

## 操作

| ボタン | 動作 |
| --- | --- |
| A / 左 | Calendar 表示中はページ切り替え。その他の表示では再描画 |
| B / 中央 | 表示モード切り替え。`Calendar -> Netwatch -> AquaPi -> Auto -> Calendar` |
| C / 右 | 表示中モードのデータを強制取得して再描画。Calendar 表示中は時刻同期 |

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

`AQUAPI_COMPACT_URL` から JSON を取得します。
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

水槽名は `short_name_ascii`、`name`、`sensor_id` の順で利用します。
温度がない場合は `--.-` と表示します。

### Auto

Auto では Calendar、Netwatch、AquaPi をローテーション表示します。
表示中は右上に `AUTO` バッジが出ます。

## 更新と再試行

- 起動時に Wi-Fi 接続と NTP 同期を行います。
- 起動時に Wi-Fi 接続できなかった場合、約 10 分ごとに再試行します。
- 時計、日付、輝度は約 1 秒ごとに確認します。
- Netwatch / AquaPi の取得は表示中のモードだけを対象にします。
- HTTP 取得失敗や JSON パース失敗は画面上にエラーとして表示します。
