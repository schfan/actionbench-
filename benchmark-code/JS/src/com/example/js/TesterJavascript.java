package com.example.js;

/*
 * Copyright (C) 2011 0xlab - http://0xlab.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by Julian Chu <walkingice@0xlab.org>
 */

import android.app.Activity;
import android.os.Bundle;
import android.os.Message;
import android.content.Intent;
import android.webkit.WebView;
import android.webkit.WebSettings;

public class TesterJavascript extends Activity {

    protected WebView mWebView;
    protected WebSettings mSettings;

    private double mTotalTime = 0.0;
    private String mResult = "";
    private String mFormattedResult = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.javascript);
        mWebView = (WebView) findViewById(R.id.web);

        mSettings = mWebView.getSettings();
        mSettings.setJavaScriptEnabled(true);
        mWebView.addJavascriptInterface(new MsgCallback(), "ANDROID_OBJ");

        mWebView.loadUrl("file:///android_asset/driver.html");
    }

    @Override
    public void onResume() {
        super.onResume();
    }


    class MsgCallback {
        public void finish(String result, String formatted_result) {
            mResult = result;
            mFormattedResult = formatted_result;
        }
    }
}