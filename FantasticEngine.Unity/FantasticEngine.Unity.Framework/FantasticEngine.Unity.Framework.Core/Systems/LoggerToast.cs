/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
using FantasticEngine.Log;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
namespace FantasticEngine
{
    /// <summary>
    /// 弹框输出日志
    /// </summary>
    public class LoggerToast : SystemBasis<LoggerToast>, Log.ILogger
    {
        public static bool bLogError
        {
            get { return GameDefines.Instance.LogError || GameDefines.Instance.LogAll; }
        }

        public static bool bLogAll
        {
            get { return GameDefines.Instance.LogAll; }
        }

        private ConcurrentQueue<KeyValuePair<string, string>>
            Messages = new ConcurrentQueue<KeyValuePair<string, string>>();

        private float _waitDuration = 0;

        public void SaveLog(string level, string message)
        {
            Messages.Enqueue(new KeyValuePair<string, string>(level, message));
        }

        private void Log(string level, string message)
        {
            switch (level)
            {
                case LogLevel.ERROR:
                    if (bLogError)
                        FETools.Toast($"<color=#ff4757>{message}</color>");
                    break;
                case LogLevel.WARN:
                    if (bLogAll)
                        FETools.Toast($"<color=#ffa502>{message}</color>");
                    break;
                case LogLevel.INFO:
                    if (bLogAll)
                        FETools.Toast($"<color=#74b9ff>{message}</color>");
                    break;
                case LogLevel.DEBUG:
                    if (bLogAll)
                        FETools.Toast(message);
                    break;
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            _waitDuration -= Time.deltaTime;
            if (_waitDuration < 0)
            {
                _waitDuration = 0.5f;
                KeyValuePair<string, string> message;
                if (UI_Tips.Root != null && Messages.Count > 0 && Messages.TryDequeue(out message))
                {
                    Log(message.Key, message.Value);
                }
            }
        }
    }
}