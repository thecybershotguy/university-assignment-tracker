#pragma once
#include <Arduino.h>

static const char SETUP_INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html>
<head>
  <meta charset="utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1"/>
  <title>University Assignment Tracker Setup</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 16px; }
    input { width: 100%; padding: 8px; margin: 6px 0 12px 0; box-sizing: border-box; }
    .row { display: grid; grid-template-columns: 1fr 2fr; gap: 12px; }
    .card { border: 1px solid #ddd; padding: 12px; border-radius: 8px; margin-bottom: 16px; }
    button { padding: 10px 14px; font-size: 16px; }
    code { background: #f4f4f4; padding: 2px 4px; border-radius: 4px; }
  </style>
</head>
<body>
  <h2>University Assignment Tracker Setup</h2>

  <div class="card">
    <p>Fill WiFi and course calendar feed URLs. Click <b>Save</b> to persist into NVS and reboot.</p>
    <p>After reboot, the device will run in normal mode.</p>
  </div>

  <form method="POST" action="/save">
    <div class="card">
      <h3>WiFi</h3>
      <label>SSID</label>
      <input name="ssid" placeholder="WiFi SSID" required value="{{SSID}}">

      <label>Password</label>
      <input name="pass" type="password" placeholder="WiFi Password" required>

      <label>Timezone Offset (seconds)</label>
      <input name="tz" placeholder="-25200 for MST (UTC-7)" value="{{TZ}}">
    </div>

    <div class="card">
      <h3>Courses (up to 8)</h3>
        {{COURSE_ROWS}}
    </div>

    <button type="submit">Save & Reboot</button>
  </form>

  <div class="card">
    <h3>Maintenance</h3>
    <form method="POST" action="/clear" onsubmit="return confirm('Clear saved config and reboot?');">
      <button type="submit">Clear Saved Config</button>
    </form>
  </div>

</body>
</html>
)HTML";