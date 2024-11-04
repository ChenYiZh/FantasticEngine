/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;


namespace FantasyEngine
{
    /// <summary>
    /// 指令参数
    /// </summary>
    public class EventParam
    {

    }

    /// <summary>
    /// 指令模式，RegistEvent和UnregistEvent绑定或解绑事件，随后在其他任意位置Send来发送指令
    /// </summary>
    public class EventSystem : SystemBasis<EventSystem>
    {
        public delegate void EventCallback(EventParam param = null);

        private Queue<KeyValuePair<EEvent, EventParam>> _msgs;

        private Dictionary<EEvent, List<CallbackInstance>> _callbacks;

        public IReadOnlyDictionary<EEvent, List<CallbackInstance>> Callbacks { get { return _callbacks; } }

        public override void Initialize()
        {
            base.Initialize();
            _callbacks = new Dictionary<EEvent, List<CallbackInstance>>();
            _msgs = new Queue<KeyValuePair<EEvent, EventParam>>();
        }

        public sealed class CallbackInstance
        {
            public bool Once { get; set; }

            public EventCallback Callback { get; set; }
        }

        public void RegistEvent(EEvent EEvent, EventCallback callback)
        {
            RegistEvent(EEvent, false, callback);
        }

        public void RegistEvent(EEvent EEvent, bool once, EventCallback callback)
        {
            if (callback == null) { return; }
            if (!_callbacks.ContainsKey(EEvent))
            {
                _callbacks.Add(EEvent, new List<CallbackInstance>());
            }
            _callbacks[EEvent].Add(new CallbackInstance()
            {
                Callback = callback,
                Once = once,
            });
        }

        public void UnregistEvent(EEvent EEvent, EventCallback callback)
        {
            if (!_callbacks.ContainsKey(EEvent))
            {
                return;
            }
            var list = _callbacks[EEvent];
            for (int i = list.Count - 1; i >= 0; i--)
            {
                if (list[i].Callback == null || list[i].Callback == callback)
                {
                    list.RemoveAt(i);
                    continue;
                }
            }
            if (list.Count == 0)
            {
                _callbacks.Remove(EEvent);
            }
        }

        public void Send(EEvent EEvent, EventParam param = null)
        {
            if (FETools.IsRunningOnGameThread())
            {
                Callback(EEvent, param);
            }
            else
            {
                _msgs.Enqueue(new KeyValuePair<EEvent, EventParam>(EEvent, param));
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (_msgs.Count > 0)
            {
                var kv = _msgs.Dequeue();
                Callback(kv.Key, kv.Value);
            }
        }

        private void Callback(EEvent EEvent, EventParam param)
        {
            if (_callbacks.ContainsKey(EEvent))
            {
                var list = _callbacks[EEvent];
                for (int i = 0; i < list.Count; i++)
                {
                    var listener = list[i];
                    if (listener.Callback != null)
                    {
                        listener.Callback.Invoke(param);
                    }
                    if (listener.Callback == null || listener.Once)
                    {
                        list.RemoveAt(i);
                        i--;
                    }
                }
                if (list.Count == 0)
                {
                    _callbacks.Remove(EEvent);
                }
            }
        }
    }
}