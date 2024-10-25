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
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace FantasyEngine.Common
{
    public interface ILazyObject : IRelease
    {
        bool Exists { get; }

        void Load();

        GameObject gameObject { get; }

        Transform transform { get; }
    }
    /// <summary>
    /// 延迟加载的数据结构，原先用于ilRuntime中
    /// </summary>
    public abstract class LazyObject : ILazyObject
    {
        public bool Exists { get; private set; }

        private GameObject _gameObject { get; set; }

        private bool _operating { get; set; }

        public GameObject gameObject
        {
            get
            {
                if (OnAnyEvent())
                {
                    return _gameObject;
                }
                else
                {
                    return null;
                }
            }
        }

        public Transform transform { get { return gameObject == null ? null : gameObject.transform; } }

        protected bool OnAnyEvent()
        {
            if (_operating)
            {
                return _gameObject;
            }
            if (Exists)
            {
                if (_gameObject)
                {
                    return true;
                }
                else
                {
#if UNITY_EDITOR
                throw new NullReferenceException();
#endif
                    return false;
                }
            }
            else
            {
                _operating = true;
                try
                {
                    _gameObject = Instantiate();
                }
                catch (Exception e)
                {
                    Debug.LogException(e);
                }
                _operating = false;
                Exists = true;
                if (_gameObject)
                {
                    return true;
                }
                else
                {
#if UNITY_EDITOR
                OnCreateFailed();
#endif
                    return false;
                }
            }
        }

        protected virtual void OnCreateFailed()
        {
            throw new NullReferenceException("GameObject 懒加载失败!");
        }

        public virtual void Release()
        {
            if (Exists)
            {
                Exists = false;
                _operating = true;
                if (_gameObject)
                {
                    try
                    {
                        OnRelease();
                    }
                    catch (Exception e)
                    {
                        Debug.LogException(e);
                    }
                    _gameObject = null;
                }
                _operating = false;
            }
        }

        public void Load()
        {
            OnAnyEvent();
        }

        protected abstract void OnRelease();

        protected abstract GameObject Instantiate();

        protected void Create()
        {
            Exists = false;
            _operating = false;
        }
    }
}