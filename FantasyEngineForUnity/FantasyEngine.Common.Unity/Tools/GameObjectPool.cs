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
    /// <summary>
    /// 对象池
    /// </summary>
    public class GameObjectPoolItem : LazyObject
    {
        public GameObjectPool Pool { get; private set; }

        private GameObject _gameObject { get; set; }

        protected GameObjectPoolItem(GameObjectPool pool)
        {
            Pool = pool;
        }

        internal static GameObjectPoolItem New(GameObjectPool pool)
        {
            return new GameObjectPoolItem(pool);
        }

        protected override GameObject Instantiate()
        {
            _gameObject = _gameObject;
            if (!_gameObject)
            {
                if (Pool.TotalCount == 0 && Pool.CanUseDefaultItem)
                {
                    _gameObject = Pool.DefaultObject;
                }
                else
                {
                    _gameObject = GameObject.Instantiate(Pool.DefaultObject, Pool.DefaultObject.transform.parent);
                }
            }
            GameObjectPool.Use(Pool, this);
            _gameObject.SetActive(true);
            return _gameObject;
        }

        protected override void OnRelease()
        {
            gameObject.SetActive(false);
            GameObjectPool.Collect(Pool, this);
        }

        internal GameObject GetGameObject()
        {
            return _gameObject;
        }
    }

    public class GameObjectPool : IRelease, IReset
    {
        internal GameObject DefaultObject { get; private set; }

        private ILazyObject _default { get; set; }

        public int TotalCount { get { return _usingItems.Count + _collection.Count; } }

        private bool _canUseDefaultItem;
        public bool CanUseDefaultItem
        {
            get { return _canUseDefaultItem; }
            set
            {
                if (value)
                {
                    if (!(_usingItems.Contains(_default) || _collection.Contains(_default)))
                    {
                        if (_default.Exists)
                        {
                            _usingItems.Add(_default);
                        }
                        else
                        {
                            _collection.Add(_default);
                        }
                    }
                }
                else
                {
                    _default.Release();
                    if (_usingItems.Contains(_default))
                    {
                        _usingItems.Remove(_default);
                    }
                    if (_collection.Contains(_default))
                    {
                        _collection.Remove(_default);
                    }
                }
            }
        }

        public Transform Parent { get; set; }

        private HashSet<ILazyObject> _usingItems;
        private HashSet<ILazyObject> _collection;

        public GameObjectPool(GameObject item, bool canUseDefaultItem = true)
        {
            DefaultObject = item;
            _usingItems = new HashSet<ILazyObject>();
            _collection = new HashSet<ILazyObject>();
            _default = GameObjectPoolItem.New(this);
            _default.Release();
            CanUseDefaultItem = canUseDefaultItem;
            _collection.Clear();
            Parent = DefaultObject.transform.parent;
        }

        public ILazyObject GetItem()
        {
            ILazyObject obj = null;
            if (_collection.Count > 0)
            {
                foreach (ILazyObject o in _collection)
                {
                    obj = o;
                    break;
                }
            }
            if (obj == null)
            {
                obj = GameObjectPoolItem.New(this);
            }
            obj.transform.SetParent(Parent);
            Use(this, obj);
            return obj;
        }

        internal static void Use(GameObjectPool pool, ILazyObject obj)
        {
            if (pool._collection.Contains(obj))
            {
                pool._collection.Remove(obj);
            }
            if (!pool._usingItems.Contains(obj))
            {
                pool._usingItems.Add(obj);
            }
        }

        internal static void Collect(GameObjectPool pool, ILazyObject obj)
        {
            if (pool._usingItems.Contains(obj))
            {
                pool._usingItems.Remove(obj);
            }
            if (!pool._collection.Contains(obj))
            {
                pool._collection.Add(obj);
            }
        }

        public void Release()
        {
            List<ILazyObject> objs = new List<ILazyObject>(_usingItems);
            foreach (ILazyObject obj in objs)
            {
                obj.gameObject.SetActive(false);
                obj.Release();
            }
        }

        public void Reset()
        {
            Release();
            List<ILazyObject> objs = new List<ILazyObject>(_collection);
            foreach (ILazyObject obj in objs)
            {
                if (obj.gameObject != DefaultObject)
                {
                    GameObject.Destroy(((GameObjectPoolItem)obj).GetGameObject());
                }
            }
            _usingItems.Clear();
            _collection.Clear();
            CanUseDefaultItem = _canUseDefaultItem;
        }
    }
}