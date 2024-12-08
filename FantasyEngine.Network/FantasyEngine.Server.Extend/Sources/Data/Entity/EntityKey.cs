/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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
using FantasyEngine.Log;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;

namespace FantasyEngine.Network.Server.Entity
{
    /// <summary>
    /// 主键管理类，主要用于Redis
    /// </summary>
    public struct EntityKey : IEntityKey
    {
        /// <summary>
        /// 数据类型
        /// </summary>
        private Type _type;

        /// <summary>
        /// 数据类型
        /// </summary>
        internal Type Type
        {
            get { return _type; }
            set
            {
                _type = null;
                RefreshKeyName();
                CheckIsSingleKey();
            }
        }

        /// <summary>
        /// 表名
        /// </summary>
        private string _tableName;

        /// <summary>
        /// 表名
        /// </summary>
        public string TableName { get { return _tableName; } }

        /// <summary>
        /// 主键
        /// </summary>
        internal object[] keys;

        /// <summary>
        /// 主键
        /// </summary>
        public IReadOnlyList<object> Keys { get { return keys; } }

        /// <summary>
        /// 完整Key名称，用于判断
        /// </summary>
        private string _keyName;

        /// <summary>
        /// 完整Key名称，用于判断
        /// </summary>
        public string KeyName { get { return _keyName; } }

        /// <summary>
        /// 全名
        /// </summary>
        private string _fullName;

        /// <summary>
        /// 通过主键生成对象
        /// </summary>
        /// <param name="keys"></param>
        public EntityKey(params object[] keys)
        {
            IsSingleKey = false;
            _type = null;
            this.keys = keys;
            _keyName = null;
            _tableName = null;
            _fullName = null;
            MakeKeyName();
        }

        /// <summary>
        /// 通过主键生成对象
        /// </summary>
        /// <param name="keys"></param>
        internal EntityKey(Type type, params object[] keys)
        {
            IsSingleKey = false;
            this._type = type;
            this.keys = keys;
            _keyName = null;
            _tableName = null;
            _fullName = null;
            MakeKeyName();
            CheckIsSingleKey();
        }

        /// <summary>
        /// 是否是主键，在作为字典Key时用来提升性能
        /// </summary>
        private bool IsSingleKey { get; set; }

        /// <summary>
        /// 判断是否是主键，在作为字典Key时用来提升性能
        /// </summary>
        private void CheckIsSingleKey()
        {
            IsSingleKey = false;
            if (_type != null)
            {
                TableScheme tableScheme = DataContext.GetTableScheme(_type) as TableScheme;
                IsSingleKey = tableScheme != null && tableScheme.KeyFields.Count == 1;
            }
        }

        /// <summary>
        /// 重新刷新KeyName
        /// </summary>
        internal void RefreshKeyName()
        {
            MakeKeyName();
        }

        /// <summary>
        /// 生成Redis遍历主键
        /// </summary>
        private void MakeKeyName()
        {
            _tableName = MakeTableName(_type);
            _keyName = MakeKeyName(keys);
            _fullName = MakeFullName(_type, keys);
        }

        /// <summary>
        /// 生成Entity的KeyName
        /// </summary>
        /// <param name="keys"></param>
        /// <returns></returns>
        public static string MakeKeyName(object[] keys)
        {
            if (keys == null) { throw new ArgumentNullException("Entity Keys is null."); }
            return string.Join(Settings.SPLITE_KEY.ToString(), keys);
            //string entityKey = string.Join(Settings.SPLITE_KEY.ToString(), keys.Select(k =>
            //{
            //    return k == null ? "" : HttpUtility.UrlEncode(k.ToString()).Replace(Settings.SPLITE_KEY.ToString(), "%" + (int)Settings.SPLITE_KEY);
            //}));
        }

        /// <summary>
        /// 生成Entity的Table名称
        /// </summary>
        public static string MakeTableName(Type type)
        {
            return type?.FullName;
        }

        /// <summary>
        /// 生成完整名称
        /// </summary>
        public static string MakeFullName(Type type, object[] keys)
        {
            return type == null ? MakeKeyName(keys) : MakeTableName(type) + Settings.SPLITE_KEY + MakeKeyName(keys);
        }

        /// <summary>
        /// 重写相等运算符
        /// </summary>
        public static bool operator ==(EntityKey key1, EntityKey key2)
        {
            if (key1.IsSingleKey && key2.IsSingleKey)
            {
                return key1.Type == key2.Type && key1.keys[0] == key2.keys[0];
            }
            return key1.ToString() == key2.ToString();
        }

        /// <summary>
        /// 重写不等运算符
        /// </summary>
        public static bool operator !=(EntityKey key1, EntityKey key2)
        {
            return !(key1 == key2);
        }

        /// <summary>
        /// 重写相等运算符
        /// </summary>
        public override bool Equals(object obj)
        {
            EntityKey? other = obj as EntityKey?;
            if (!other.HasValue)
            {
                FEConsole.Write("!other.HasValue");
                return false;
            }
            if (IsSingleKey && other.Value.IsSingleKey)
            {
                return _type == other.Value._type && keys[0].Equals(other.Value.keys[0]);
            }
            return ToString() == obj.ToString();
        }

        /// <summary>
        /// HashCode是否要重写？
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return ToString().GetHashCode();
        }

        /// <summary>
        /// 重写ToString()
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return _fullName;
        }

        /// <summary>
        /// 隐式转换成字符串
        /// </summary>
        /// <param name="key"></param>
        public static implicit operator string(EntityKey key)
        {
            return key.ToString();
        }
    }
}
