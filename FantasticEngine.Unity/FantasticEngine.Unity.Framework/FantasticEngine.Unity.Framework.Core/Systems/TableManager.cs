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
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using FantasticEngine.Log;
using FantasticEngine.Security;
using UnityEngine;
using UnityEngine.Networking;
namespace FantasticEngine
{
    /// <summary>
    /// 自动读表结构
    /// </summary>
    public abstract class BaseTableReader
    {
        public abstract string FileName { get; }

        /// <summary>
        /// 初始化完成后调用，读完表直接调用
        /// </summary>
        protected virtual void OnInitialized()
        {
        }

        private StringBuilder _Errors;

        public StringBuilder Errors
        {
            get
            {
                if (_Errors == null)
                {
                    _Errors = new StringBuilder();
                }

                return _Errors;
            }
        }

        public static void EndingInit(BaseTableReader reader)
        {
            try
            {
                reader.OnInitialized();
            }
            catch (Exception e)
            {
                reader.Errors.AppendLine(reader.FileName + ": " + e.Message);
                reader.Errors.AppendLine(e.StackTrace);
            }
        }

        public abstract void LoadData(TableInfo tableInfo);
    }
    /// <summary>
    /// 表结构
    /// </summary>
    public abstract class BaseTableReader<T> : BaseTableReader where T : BaseTableData, new()
    {
        private Dictionary<uint, T> _dic;

        public void Initialize()
        {
            _dic = new Dictionary<uint, T>();
            TableManager.Instance.Push(this);
        }

        public IEnumerable<uint> Keys
        {
            get { return _dic.Keys; }
        }

        public IEnumerable<T> Values
        {
            get { return ToArray(); }
        }

        /// <summary>
        /// 内部实现使用GetData(uint key)
        /// </summary>
        public virtual T GetData(int key)
        {
            return GetData((uint)key);
        }

        public virtual T GetData(uint key, bool force = false)
        {
            if (_dic == null && !force)
            {
                //Errors.AppendLine(FileName + "生命周期有误，在表初始化之前被调用");
                return null;
            }

            if (_dic.ContainsKey(key))
            {
                return _dic[key];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// 内部实现使用GetData(uint key)
        /// </summary>
        public virtual T this[int key]
        {
            get { return GetData(key); }
        }

        public virtual T this[uint key]
        {
            get { return GetData(key); }
        }

        public IReadOnlyList<T> ToList()
        {
            if (_dic == null)
            {
                return null;
            }

            List<T> list = new List<T>();
            foreach (uint key in _dic.Keys)
            {
                list.Add(GetData(key));
            }

            return list;
        }

        public int Count
        {
            get
            {
                if (_dic == null)
                {
                    return 0;
                }
                else
                {
                    return _dic.Count;
                }
            }
        }

        public T[] ToArray()
        {
            if (_dic == null)
            {
                return null;
            }

            T[] array = new T[Count];
            int index = 0;
            foreach (uint key in Keys)
            {
                array[index] = GetData(key);
                index++;
            }

            return array;
        }

        public IReadOnlyDictionary<uint, T> ToDictionary()
        {
            Dictionary<uint, T> dic = new Dictionary<uint, T>();
            foreach (uint key in Keys)
            {
                dic.Add(key, GetData(key));
            }

            return dic;
        }

        public bool ContainsKey(uint key)
        {
            if (_dic == null)
            {
                return false;
            }

            return _dic.ContainsKey(key);
        }

        public bool ContainsKey(int key)
        {
            if (_dic == null)
            {
                return false;
            }

            return _dic.ContainsKey((uint)key) && _dic[(uint)key] != null;
        }

        public override void LoadData(TableInfo tableInfo)
        {
            if (_dic == null)
            {
                _dic = new Dictionary<uint, T>();
            }

            _dic.Clear();
            //WebGL下如果没有Add过会变成空
            if (tableInfo.Cache == null)
            {
                return;
            }

            foreach (var cache in tableInfo.Cache)
            {
                T t = new T();
                //FEConsole.WriteWarn("StartLoad: " + tableInfo.FileName + ", " + cache.Key);
                if (cache.Value == null || !t.Load(cache.Value, tableInfo.Errors))
                {
                    tableInfo.Errors.AppendLine($"{FileName}行：{cache.Key}读表错误");
                    //FEConsole.WriteWarn($"{FileName}行：{cache.Key}读表错误");
                    //FEConsole.WriteWarn("EndLoad: " + tableInfo.FileName);
                    continue;
                }

                _dic.Add(cache.Key, t);
                //FEConsole.WriteWarn("EndLoad: " + tableInfo.FileName);
            }
        }
    }

    public class TableManagerClass<T> : SystemBasis<T> where T : TableManagerClass<T>, new()
    {
        protected virtual void OnInitialize() { }
    }
    /// <summary>
    /// 表管理
    /// </summary>
    public partial class TableManager : TableManagerClass<TableManager>
    {
        private Dictionary<string, BaseTableReader> _readers;

        public TableLoaderConfig Config { get; set; }

        private bool Initialized { get; set; } = false;

        public override bool UpdateBeforeInitialze
        {
            get { return true; }
        }

        public float Progress { get; private set; }

        public override bool Ready
        {
            get { return Initialized; }
        }

        public TableManager()
        {
            Initialized = false;
            _readers = new Dictionary<string, BaseTableReader>();
        }

        public void Push(BaseTableReader reader)
        {
            if (_readers == null)
            {
                return;
            }

            _readers.Add(reader.FileName, reader);
        }

        public void Load(TableLoaderConfig config = null)
        {
            Initialized = false;
            _readers.Clear();
            if (config != null)
            {
                Config = config;
            }

            float current = UnityEngine.Time.time;
            OnInitialize();
            FEConsole.WriteInfo("初始化表花费时间：" + (Time.time - current) + "(s)");
            List<string> tableCache = new List<string>();
            foreach (var table in _readers)
            {
                tableCache.Add(table.Key);
            }

            Loading(tableCache);
        }

        private void Loading(List<string> orderList)
        {
            Initialized = false;
            logTime = Time.time;
            GameRoot.Root.StartCoroutine(LoadTables(orderList));
        }

        float logTime;

        private float _lastProgress;

        private IEnumerator OnLoaded()
        {
            float current = Time.time;

            foreach (BaseTableReader reader in _readers.Values)
            {
                BaseTableReader.EndingInit(reader);
            }

            foreach (BaseTableReader reader in _readers.Values)
            {
                if (reader.Errors != null && reader.Errors.Length > 0)
                {
                    FEConsole.WriteError(reader.FileName + ": \n" + reader.Errors.ToString());
                }
            }

            _readers.Clear();
            Initialized = true;
            FEConsole.WriteInfo("最后读表花费时间：" + (Time.time - current) + "(s)");
            //LabelManager.Instance.Refresh();
            //EventManager.Instance.Send(Events.Ready);
            yield return null;
        }

        #region Threading

        private IEnumerator LoadTables(List<string> tables)
        {
#if UNITY_EDITOR
        System.Diagnostics.Stopwatch totalWatch = new System.Diagnostics.Stopwatch();
        totalWatch.Start();
#endif
            Progress = 0;
            Dictionary<string, TableInfo> _tables = new Dictionary<string, TableInfo>();
            string md5 = null;
            string idxPath = Config.BasePath + "/assets.idx";
            string assetInfo = null;
            TextAsset asset = null;
            bool wait2SaveIdx = false;
            if (!Config.UseLocalTable && Exists(idxPath))
            {
                assetInfo = ReadAllText(idxPath);
                _tables = LoadIdx(assetInfo);
            }
            else
            {
                asset = Resources.Load<TextAsset>("assets");
                assetInfo = asset.text;
                Resources.UnloadAsset(asset);
                _tables = LoadIdx(assetInfo);
                wait2SaveIdx = true;
            }

            if (!Config.UseLocalTable)
            {
                idxPath = Config.IdxUrl;
                using (UnityWebRequest request =
                       UnityWebRequest.Get(idxPath + "?" + DateTime.Now.ToString("yyyyMMddHHmmss")))
                {
#if UNITY_2018_OR_NEWER
                    request.SendWebRequest();
#else
                    request.Send();
#endif
                    while (!request.isDone)
                    {
                        yield return null;
                    }
#if UNITY_2018_OR_NEWER
                    if (request.isNetworkError || request.isHttpError)
#else
                    if (request.isError)
#endif
                    {
                        FEConsole.WriteError("网络连接异常！");
                    }
                    else
                    {
                        List<TableInfo> wait2RemoveTables = new List<TableInfo>();
                        string onlineTxt = request.downloadHandler.text;
                        Dictionary<string, TableInfo> onlineTables = LoadIdx(onlineTxt);
                        assetInfo = onlineTxt;
                        wait2SaveIdx = true;
                        foreach (string key in _tables.Keys)
                        {
                            if (!onlineTables.ContainsKey(key))
                            {
                                wait2RemoveTables.Add(_tables[key]);
                            }
                        }

                        foreach (TableInfo table in wait2RemoveTables)
                        {
                            _tables.Remove(table.FileName);
                            string tablePath = Config.GetLocalTablePath(table.FileName);
                            if (Exists(tablePath))
                            {
                                File.Delete(tablePath);
                            }
                        }

                        foreach (KeyValuePair<string, TableInfo> table in onlineTables)
                        {
                            if (_tables.ContainsKey(table.Key))
                            {
                                if (_tables[table.Key].MD5 != table.Value.MD5)
                                {
                                    _tables[table.Key].Different = true;
                                    wait2SaveIdx = true;
                                }
                            }
                            else
                            {
                                _tables.Add(table.Key, table.Value);
                                table.Value.Different = true;
                                wait2SaveIdx = true;
                            }
                        }
                    }
                }
            }

#if !UNITY_WEBGL
            if (wait2SaveIdx)
            {
                WriteAllText(Config.BasePath + "/assets.idx", assetInfo);
            }
#endif

#if UNITY_EDITOR
        FEConsole.WriteInfo("开始读表！");
        System.Diagnostics.Stopwatch tableWatch = new System.Diagnostics.Stopwatch();
#endif

            yield return StartLoading(_tables);

#if UNITY_EDITOR
        totalWatch.Stop();
        FEConsole.WriteInfo("读表总耗时: " + totalWatch.Elapsed.TotalSeconds.ToString("f2") + "s");
#endif
        }

        private IEnumerator StartLoading(Dictionary<string, TableInfo> _tables)
        {
            List<string> keys = new List<string>(_tables.Keys);
            Dictionary<string, string> caches = new Dictionary<string, string>();
            foreach (string key in keys)
            {
                caches.Add(key, null);
            }
#if !UNITY_WEBGL
            int index = 0;
#endif
            float current = Time.time;
            //FEConsole.WriteInfo("different:" + _tables.Values.Where(t => t.Different).Count());

            for (int k = 0; k < keys.Count; k++)
            {
                string filename = keys[k];
#if !UNITY_WEBGL
                new Thread(() =>
                {
#endif
                    try
                    {
                        TableInfo table = _tables[filename];
#if UNITY_WEBGL
                table.Different = true;
                caches[filename] = null;
                continue;
#endif
                        //table.Different = true;
                        string tablePath = Config.GetLocalTablePath(table.FileName);
                        string content = null;

                        if (!table.Different && Exists(tablePath))
                        {
                            byte[] buffer = File.ReadAllBytes(tablePath);
                            System.Security.Cryptography.MD5 Cryptor =
                            new System.Security.Cryptography.MD5CryptoServiceProvider();
                            byte[] bytes = Cryptor.ComputeHash(buffer);
                            StringBuilder builder = new StringBuilder();
                            for (int i = 0; i < bytes.Length; i++)
                            {
                                builder.Append(bytes[i].ToString("x2"));
                            }

                            if (table.MD5 != builder.ToString())
                            {
                                table.Different = true;
                                content = null;
                            }
                            else
                            {
                                content = Encoding.UTF8.GetString(buffer);
                                //TableLogError(table, string.Format("{0}|{1}\n{2}", filename, @tablePath, caches[filename]));
                            }
                        }

                        caches[filename] = content;
                    }
                    catch (Exception ex)
                    {
                    }
#if !UNITY_WEBGL
                    Interlocked.Increment(ref index);
                }).Start();
#endif
            }
#if !UNITY_WEBGL
            while (index < _tables.Count)
            {
                yield return null;
            }
#endif
            FEConsole.WriteInfo("GetLocalTablePath:" + (Time.time - current) + "(s)");

            current = Time.time;
            //FEConsole.WriteInfo("Tables File Different Count:" + _tables.Values.Where(t => t.Different).Count());
            for (int k = 0; k < keys.Count; k++)
            {
                string filename = keys[k];
                TableInfo table = _tables[filename];
                string tablePath = Config.GetLocalTablePath(table.FileName);
                if ((!table.Different && !Exists(tablePath)) ||
                    ((!Application.isEditor || string.IsNullOrEmpty(caches[filename])) && Config.UseLocalTable))
                {
                    var asset = Resources.Load<TextAsset>(Config.TablePath + table.FileName);
                    if (asset != null)
                    {
                        caches[filename] = asset.text;
#if !UNITY_WEBGL
                        WriteAllBytes(@tablePath, asset.bytes);
#endif
                        Resources.UnloadAsset(asset);
                    }
                    else
                    {
                        TableLogError(table, "未找到表: " + table.FileName);
                        _tables.Remove(filename);
                        table.Different = true;
                    }
                }
            }

            FEConsole.WriteInfo("Resources.Load:" + (Time.time - current) + "(s)");
            current = Time.time;
#if !UNITY_WEBGL
            index = 0;
#endif
            for (int k = 0; k < keys.Count; k++)
            {
                string filename = keys[k];
#if !UNITY_WEBGL
                new Thread(() =>
                {
#endif
                    TableInfo table = _tables[filename];

#if UNITY_WEBGL
            if (table == null)
            {
                continue;
            }
#endif

                    try
                    {
#if !UNITY_WEBGL
                        string tablePath = Config.GetLocalTablePath(table.FileName);
                        if (table.Different && !Config.UseLocalTable)
                        {
                            try
                            {
                                table.Errors.AppendLine($"下载在线表：{filename}");
                                HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(
                                Config.GetOnlineTablePath(table.FileName) + "?" +
                                DateTime.Now.ToString("yyyyMMddHHmmss"));
                                HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                                using (Stream stream = response.GetResponseStream())
                                {
                                    using (StreamReader reader = new StreamReader(stream, Encoding.UTF8))
                                    {
                                        caches[filename] = reader.ReadToEnd();
                                        WriteAllText(tablePath, caches[filename]);
                                    }
                                }
                            }
                            catch (WebException e)
                            {
                                table.Errors.AppendLine($"{e.Message}");
                            }
                            catch (Exception e2)
                            {
                                table.Errors.AppendLine($"{e2.Message}");
                            }
                        }
#endif
                        string content = caches[filename];

#if !UNITY_EDITOR && !UNITY_WEBGL
                        if (string.IsNullOrEmpty(content) && Exists(tablePath))
                        {
                            content = ReadAllText(tablePath);
                        }
#endif

                        if (!string.IsNullOrEmpty(content))
                        {
                            table.Load(DoFastEncryptCaesar(content, -Config.Shift));
                        }

                        BaseTableReader table_Ins = _readers[filename];
                        if (table_Ins != null)
                        {
                            table_Ins.LoadData(table);
                        }
                    }
                    catch (Exception ex)
                    {
                        table.Errors.AppendLine($"{ex.Message}");
                    }
#if !UNITY_WEBGL
                    Interlocked.Increment(ref index);
                }).Start();
#endif
            }
#if !UNITY_WEBGL
            while (index < keys.Count)
            {
                Progress = index * 1.0f / _tables.Count;
                yield return null;
            }
#endif
            FEConsole.WriteInfo("DoFastEncryptCaesar:" + (UnityEngine.Time.time - current) + "(s)");
            foreach (KeyValuePair<string, TableInfo> table in _tables)
            {
                if (table.Value.Errors != null && table.Value.Errors.Length > 0)
                {
#if UNITY_EDITOR
                FEConsole.WriteError(table.Key + "解析错误\n" + table.Value.Errors.ToString());
#endif
                    table.Value.Errors = null;
                }
            }

            FEConsole.WriteInfo("主工程读表时间：" + (UnityEngine.Time.time - logTime) + "(s)");
            yield return OnLoaded();
        }

        private void TableLogError(TableInfo table, string str)
        {
            if (table != null && table.Errors != null)
            {
                table.Errors.AppendLine(str);
            }
        }

        private bool Exists(string path)
        {
            return File.Exists(@path);
        }

        private Dictionary<string, TableInfo> LoadIdx(string text)
        {
            Dictionary<string, TableInfo> idx = new Dictionary<string, TableInfo>();
            string[] lines = text.Split('\n');
            if (lines.Length == 0)
            {
                throw new System.Exception("表解析异常");
            }

            foreach (string l in lines)
            {
                string line = l.Replace("\r", "");
                string[] data = line.Split('|');
                if (data.Length == 2)
                {
                    TableInfo table = new TableInfo(data);
                    if (!string.IsNullOrWhiteSpace(table.FileName) && !string.IsNullOrWhiteSpace(table.MD5))
                    {
                        if (idx.ContainsKey(table.FileName))
                        {
                            FEConsole.WriteError("发现重名表");
                        }
                        else
                        {
                            idx.Add(table.FileName, table);
                        }
                    }
                }
            }

            return idx;
        }

        /// <summary>
        /// 加密解密方式
        /// </summary>
        /// <param name="text"></param>
        /// <param name="shift"></param>
        /// <returns></returns>
        public static string DoFastEncryptCaesar(string text, int shift)
        {
            return FastObfuscation.Shift(ref text, shift);
            //StringBuilder _sb_FastEncryptCaesar = new StringBuilder();
            //_sb_FastEncryptCaesar.Length = 0;
            //if (!string.IsNullOrEmpty(text) && text.Length > 0)
            //{
            //    char c = (char)((text[0] + shift) % char.MaxValue);
            //    if (!((c < '0' || c > '9') && c != '\t' && c != '\r' && c != '\n'))
            //    {
            //        _sb_FastEncryptCaesar.Append(c);
            //    }
            //}

            //for (int i = 1; i < text.Length; i++)
            //    _sb_FastEncryptCaesar.Append((char)((text[i] + shift) % char.MaxValue));
            //return _sb_FastEncryptCaesar.ToString();
        }

        private void WriteAllBytes(string path, byte[] buffer)
        {
            FileUtility.SaveFile(path, buffer);
        }

        private string ReadAllText(string path)
        {
            return FileUtility.ReadFileText(path);
        }

        private void WriteAllText(string path, string text)
        {
            FileUtility.SaveFile(path, text);
        }

        #endregion
    }

    public sealed class TableInfo
    {
        public string FileName { get; private set; }
        public string MD5 { get; private set; }
        public bool Different { get; set; }
        private Dictionary<uint, string[]> _cache;

        public IReadOnlyDictionary<uint, string[]> Cache
        {
            get { return _cache; }
        }

        public StringBuilder Errors { get; internal set; }

        public TableInfo(string[] data)
        {
            FileName = data[0];
            MD5 = data[1];
            Different = false;
            Errors = new StringBuilder();
        }

        public void Load(string text)
        {
            _cache = new Dictionary<uint, string[]>();
            string[] lines = text.Split('\n');
            for (int i = 0; i < lines.Length; i++)
            {
                string[] values = lines[i].Replace("\r", "").Split('\t');
                uint key;
                if (!string.IsNullOrEmpty(values[0]) && uint.TryParse(values[0], out key))
                {
                    _cache.Add(key, values);
                }
                else
                {
                    Errors.AppendLine(FileName + " 解析异常 行: " + i + "\n" + string.Join("\t", values));
                }
            }
        }
    }

    public class TableLoaderConfig
    {
        public string TablePath { get; set; }
        public bool UseLocalTable { get; set; }
        public string IdxUrl { get; set; }
        public string TablesUrl { get; set; }
        public string BasePath { get; set; }

        public int Shift { get; set; }

        public bool TraceNullValue { get; set; }

        public string GetOnlineTablePath(string tableFileName)
        {
            return TablesUrl + tableFileName + ".zip";
        }

        public string GetLocalTablePath(string tableFileName)
        {
            string path = BasePath + "/" + TablePath + tableFileName;
            path += ".zip";
            return path;
        }
    }

    public abstract class BaseTableData
    {
        protected abstract bool Read(string[] values);

        public bool Load(string[] values, StringBuilder builder = null)
        {
            bool result = false;
            try
            {
                result = Read(values);
                OnReaded(values);
            }
            catch (System.Exception e)
            {
                if (builder != null)
                {
                    builder.AppendLine(string.Format("[{0}]: {1}: ", values.Length > 0 ? values[0].ToString() : "空行",
                        e.Message));
                    builder.AppendLine(e.StackTrace);
                }
            }

            return result;
        }

        protected virtual void OnReaded(string[] values)
        {
        }

        public Vector2 ToVector2(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return Vector2.zero;
            }

            string[] values = value.Split(',');
            return values.Length > 1 ? new Vector2(float.Parse(values[0]), float.Parse(values[1])) : Vector2.zero;
        }

        public Vector2[] ToVector2Array(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return new Vector2[0];
            }

            string[] items = value.Split(';');
            Vector2[] vs = new Vector2[items.Length];
            for (int i = 0; i < vs.Length; i++)
            {
                vs[i] = ToVector2(items[i]);
            }

            return vs;
        }

        public List<Vector2[]> ToVector2ListArray(string value)
        {
            List<Vector2[]> result = new List<Vector2[]>();
            if (string.IsNullOrEmpty(value))
            {
                return result;
            }

            string[] list = value.Split('%');
            foreach (string l in list)
            {
                result.Add(ToVector2Array(l));
            }

            return result;
        }

        public Vector3 ToVector3(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return Vector3.zero;
            }

            string[] values = value.Split(',');
            return values.Length > 2 ? new Vector3(float.Parse(values[0]), float.Parse(values[1])) : Vector3.zero;
        }

        public Vector3[] ToVector3Array(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return new Vector3[0];
            }

            string[] items = value.Split(';');
            Vector3[] vs = new Vector3[items.Length];
            for (int i = 0; i < vs.Length; i++)
            {
                vs[i] = ToVector3(items[i]);
            }

            return vs;
        }

        public Dictionary<int, int> ToIntDic(string value)
        {
            Dictionary<int, int> dic = new Dictionary<int, int>();
            if (string.IsNullOrEmpty(value))
            {
                return dic;
            }

            string[] lines = value.Split(';');
            foreach (string line in lines)
            {
                string[] values = line.Split(',');
                if (values.Length > 1)
                {
                    dic.Add(int.Parse(values[0]), int.Parse(values[1]));
                }
            }

            return dic;
        }


        public Dictionary<string, string> ToStringDic(string value)
        {
            Dictionary<string, string> dic = new Dictionary<string, string>();
            if (string.IsNullOrEmpty(value))
            {
                return dic;
            }

            string[] lines = value.Split(';');
            foreach (string line in lines)
            {
                string[] values = line.Split(',');
                if (values.Length > 1)
                {
                    dic.Add(values[0], values[1]);
                }
            }

            return dic;
        }

        #region 解析函数

        protected static bool TryConvertString(string[] values, int index, out string result)
        {
            if (values.Length > index)
            {
                result = Replace(values[index]);
                return true;
            }
            else
            {
                result = null;
                return false;
            }
        }

        protected static bool TryConvertShort(string[] values, int index, short defaultValue, out short result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (short.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertInt(string[] values, int index, int defaultValue, out int result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (int.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertLong(string[] values, int index, long defaultValue, out long result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (long.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertUShort(string[] values, int index, ushort defaultValue, out ushort result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (ushort.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertUInt(string[] values, int index, uint defaultValue, out uint result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (uint.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertULong(string[] values, int index, ulong defaultValue, out ulong result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (ulong.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertFloat(string[] values, int index, float defaultValue, out float result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (float.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertDouble(string[] values, int index, double defaultValue, out double result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (double.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertBool(string[] values, int index, bool defaultValue, out bool result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (bool.TryParse(values[index], out result))
                {
                    return true;
                }
                else
                {
                    if (values[index] == "1")
                    {
                        result = true;
                        return true;
                    }
                    else if (values[index] == "0")
                    {
                        result = false;
                        return true;
                    }
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertByte(string[] values, int index, byte defaultValue, out byte result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (byte.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertChar(string[] values, int index, char defaultValue, out char result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = defaultValue;
                    return true;
                }

                if (char.TryParse(values[index], out result))
                {
                    return true;
                }

                result = defaultValue;
                return false;
            }
            else
            {
                result = defaultValue;
                return false;
            }
        }

        protected static bool TryConvertStringArray(string[] values, int index, out string[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<string>();
                    return true;
                }

                return TryConvertStringArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<string>();
                return true;
            }
        }

        private static bool TryConvertStringArray(string values, out string[] result)
        {
            if (values != null)
            {
                result = values.Split(',');
                for (int i = 0; i < result.Length; i++)
                {
                    result[i] = Replace(result[i]);
                }

                return true;
            }
            else
            {
                result = DefaultArray<string>();
                return true;
            }
        }

        protected static bool TryConvertShortArray(string[] values, int index, out short[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<short>();
                    return true;
                }

                return TryConvertShortArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<short>();
                return false;
            }
        }

        private static bool TryConvertShortArray(string values, out short[] result)
        {
            string[] items = values.Split(',');
            result = new short[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!short.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<short>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertIntArray(string[] values, int index, out int[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<int>();
                    return true;
                }

                return TryConvertIntArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<int>();
                return false;
            }
        }

        private static bool TryConvertIntArray(string values, out int[] result)
        {
            string[] items = values.Split(',');
            result = new int[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!int.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<int>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertLongArray(string[] values, int index, out long[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<long>();
                    return true;
                }

                return TryConvertLongArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<long>();
                return false;
            }
        }

        private static bool TryConvertLongArray(string values, out long[] result)
        {
            string[] items = values.Split(',');
            result = new long[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!long.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<long>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertUShortArray(string[] values, int index, out ushort[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<ushort>();
                    return true;
                }

                return TryConvertUShortArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<ushort>();
                return false;
            }
        }

        private static bool TryConvertUShortArray(string values, out ushort[] result)
        {
            string[] items = values.Split(',');
            result = new ushort[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!ushort.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<ushort>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertUIntArray(string[] values, int index, out uint[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<uint>();
                    return true;
                }

                return TryConvertUIntArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<uint>();
                return false;
            }
        }

        private static bool TryConvertUIntArray(string values, out uint[] result)
        {
            string[] items = values.Split(',');
            result = new uint[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!uint.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<uint>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertULongArray(string[] values, int index, out ulong[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<ulong>();
                    return true;
                }

                return TryConvertULongArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<ulong>();
                return false;
            }
        }

        private static bool TryConvertULongArray(string values, out ulong[] result)
        {
            string[] items = values.Split(',');
            result = new ulong[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!ulong.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<ulong>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertFloatArray(string[] values, int index, out float[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<float>();
                    return true;
                }

                return TryConvertFloatArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<float>();
                return false;
            }
        }

        private static bool TryConvertFloatArray(string values, out float[] result)
        {
            string[] items = values.Split(',');
            result = new float[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!float.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<float>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertDoubleArray(string[] values, int index, out double[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<double>();
                    return true;
                }

                return TryConvertDoubleArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<double>();
                return false;
            }
        }

        private static bool TryConvertDoubleArray(string values, out double[] result)
        {
            string[] items = values.Split(',');
            result = new double[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!double.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<double>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertBoolArray(string[] values, int index, out bool[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<bool>();
                    return true;
                }

                return TryConvertBoolArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<bool>();
                return false;
            }
        }

        private static bool TryConvertBoolArray(string values, out bool[] result)
        {
            string[] items = values.Split(',');
            result = new bool[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!bool.TryParse(items[i], out result[i]))
                {
                    if (items[i] == "1")
                    {
                        result[i] = true;
                    }
                    else if (items[i] == "0")
                    {
                        result[i] = false;
                    }
                    else
                    {
                        result = DefaultArray<bool>();
                        return false;
                    }
                }
            }

            return true;
        }

        protected static bool TryConvertByteArray(string[] values, int index, out byte[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<byte>();
                    return true;
                }

                return TryConvertByteArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<byte>();
                return false;
            }
        }

        private static bool TryConvertByteArray(string values, out byte[] result)
        {
            string[] items = values.Split(',');
            result = new byte[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!byte.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<byte>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertCharArray(string[] values, int index, out char[] result)
        {
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<char>();
                    return true;
                }

                return TryConvertCharArray(values[index], out result);
            }
            else
            {
                result = DefaultArray<char>();
                return false;
            }
        }

        private static bool TryConvertCharArray(string values, out char[] result)
        {
            string[] items = values.Split(',');
            result = new char[items.Length];
            for (int i = 0; i < result.Length; i++)
            {
                if (!char.TryParse(items[i], out result[i]))
                {
                    result = DefaultArray<char>();
                    return false;
                }
            }

            return true;
        }

        protected static bool TryConvertStringArrayArray(string[] values, int index, out string[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<string[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                string[][] res = new string[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    string[] value = null;
                    if (!TryConvertStringArray(items[i], out value))
                    {
                        result = DefaultArray<string[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<string[]>();
                return true;
            }
        }

        protected static bool TryConvertShortArrayArray(string[] values, int index, out short[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<short[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                short[][] res = new short[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    short[] value = null;
                    if (!TryConvertShortArray(items[i], out value))
                    {
                        result = DefaultArray<short[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<short[]>();
                return false;
            }
        }

        protected static bool TryConvertIntArrayArray(string[] values, int index, out int[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<int[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                int[][] res = new int[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    int[] value = null;
                    if (!TryConvertIntArray(items[i], out value))
                    {
                        result = DefaultArray<int[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<int[]>();
                return false;
            }
        }

        protected static bool TryConvertLongArrayArray(string[] values, int index, out long[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<long[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                long[][] res = new long[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    long[] value = null;
                    if (!TryConvertLongArray(items[i], out value))
                    {
                        result = DefaultArray<long[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<long[]>();
                return false;
            }
        }

        protected static bool TryConvertUShortArrayArray(string[] values, int index, out ushort[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<ushort[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                ushort[][] res = new ushort[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    ushort[] value = null;
                    if (!TryConvertUShortArray(items[i], out value))
                    {
                        result = DefaultArray<ushort[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<ushort[]>();
                return false;
            }
        }

        protected static bool TryConvertUIntArrayArray(string[] values, int index, out uint[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<uint[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                uint[][] res = new uint[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    uint[] value = null;
                    if (!TryConvertUIntArray(items[i], out value))
                    {
                        result = DefaultArray<uint[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<uint[]>();
                return false;
            }
        }

        protected static bool TryConvertULongArrayArray(string[] values, int index, out ulong[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<ulong[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                ulong[][] res = new ulong[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    ulong[] value = null;
                    if (!TryConvertULongArray(items[i], out value))
                    {
                        result = DefaultArray<ulong[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<ulong[]>();
                return false;
            }
        }

        protected static bool TryConvertFloatArrayArray(string[] values, int index, out float[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<float[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                float[][] res = new float[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    float[] value = null;
                    if (!TryConvertFloatArray(items[i], out value))
                    {
                        result = DefaultArray<float[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<float[]>();
                return false;
            }
        }

        protected static bool TryConvertDoubleArrayArray(string[] values, int index, out double[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<double[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                double[][] res = new double[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    double[] value = null;
                    if (!TryConvertDoubleArray(items[i], out value))
                    {
                        result = DefaultArray<double[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<double[]>();
                return false;
            }
        }

        protected static bool TryConvertBoolArrayArray(string[] values, int index, out bool[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<bool[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                bool[][] res = new bool[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    bool[] value = null;
                    if (!TryConvertBoolArray(items[i], out value))
                    {
                        result = DefaultArray<bool[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<bool[]>();
                return false;
            }
        }

        protected static bool TryConvertByteArrayArray(string[] values, int index, out byte[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<byte[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                byte[][] res = new byte[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    byte[] value = null;
                    if (!TryConvertByteArray(items[i], out value))
                    {
                        result = DefaultArray<byte[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<byte[]>();
                return false;
            }
        }

        protected static bool TryConvertCharArrayArray(string[] values, int index, out char[][] result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultArray<char[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                char[][] res = new char[items.Length][];
                for (int i = 0; i < items.Length; i++)
                {
                    char[] value = null;
                    if (!TryConvertCharArray(items[i], out value))
                    {
                        result = DefaultArray<char[]>();
                        return false;
                    }

                    res[i] = value;
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultArray<char[]>();
                return false;
            }
        }

        protected static bool TryConvertStringListArray(string[] values, int index, out List<string[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<string[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<string[]> res = new List<string[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    string[] value = null;
                    if (!TryConvertStringArray(items[i], out value))
                    {
                        result = DefaultList<string[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<string[]>();
                return false;
            }
        }

        protected static bool TryConvertShortListArray(string[] values, int index, out List<short[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<short[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<short[]> res = new List<short[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    short[] value = null;
                    if (!TryConvertShortArray(items[i], out value))
                    {
                        result = DefaultList<short[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<short[]>();
                return false;
            }
        }

        protected static bool TryConvertIntListArray(string[] values, int index, out List<int[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<int[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<int[]> res = new List<int[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    int[] value = null;
                    if (!TryConvertIntArray(items[i], out value))
                    {
                        result = DefaultList<int[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<int[]>();
                return false;
            }
        }

        protected static bool TryConvertLongListArray(string[] values, int index, out List<long[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<long[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<long[]> res = new List<long[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    long[] value = null;
                    if (!TryConvertLongArray(items[i], out value))
                    {
                        result = DefaultList<long[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<long[]>();
                return false;
            }
        }

        protected static bool TryConvertUShortListArray(string[] values, int index, out List<ushort[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<ushort[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<ushort[]> res = new List<ushort[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    ushort[] value = null;
                    if (!TryConvertUShortArray(items[i], out value))
                    {
                        result = DefaultList<ushort[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<ushort[]>();
                return false;
            }
        }

        protected static bool TryConvertUIntListArray(string[] values, int index, out List<uint[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<uint[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<uint[]> res = new List<uint[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    uint[] value = null;
                    if (!TryConvertUIntArray(items[i], out value))
                    {
                        result = DefaultList<uint[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<uint[]>();
                return false;
            }
        }

        protected static bool TryConvertULongListArray(string[] values, int index, out List<ulong[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<ulong[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<ulong[]> res = new List<ulong[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    ulong[] value = null;
                    if (!TryConvertULongArray(items[i], out value))
                    {
                        result = DefaultList<ulong[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<ulong[]>();
                return false;
            }
        }

        protected static bool TryConvertFloatListArray(string[] values, int index, out List<float[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<float[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<float[]> res = new List<float[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    float[] value = null;
                    if (!TryConvertFloatArray(items[i], out value))
                    {
                        result = DefaultList<float[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<float[]>();
                return false;
            }
        }

        protected static bool TryConvertDoubleListArray(string[] values, int index, out List<double[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<double[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<double[]> res = new List<double[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    double[] value = null;
                    if (!TryConvertDoubleArray(items[i], out value))
                    {
                        result = DefaultList<double[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<double[]>();
                return false;
            }
        }

        protected static bool TryConvertBoolListArray(string[] values, int index, out List<bool[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<bool[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<bool[]> res = new List<bool[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    bool[] value = null;
                    if (!TryConvertBoolArray(items[i], out value))
                    {
                        result = DefaultList<bool[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<bool[]>();
                return false;
            }
        }

        protected static bool TryConvertByteListArray(string[] values, int index, out List<byte[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<byte[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<byte[]> res = new List<byte[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    byte[] value = null;
                    if (!TryConvertByteArray(items[i], out value))
                    {
                        result = DefaultList<byte[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<byte[]>();
                return false;
            }
        }

        protected static bool TryConvertCharListArray(string[] values, int index, out List<char[]> result)
        {
            result = null;
            if (values.Length > index)
            {
                if (string.IsNullOrEmpty(values[index]))
                {
                    result = DefaultList<char[]>();
                    return true;
                }

                string[] items = values[index].Split(';');
                List<char[]> res = new List<char[]>();
                for (int i = 0; i < items.Length; i++)
                {
                    char[] value = null;
                    if (!TryConvertCharArray(items[i], out value))
                    {
                        result = DefaultList<char[]>();
                        return false;
                    }

                    res.Add(value);
                }

                result = res;
                return true;
            }
            else
            {
                result = DefaultList<char[]>();
                return false;
            }
        }

        private static List<T> DefaultList<T>()
        {
            return new List<T>();
        }

        private static T[] DefaultArray<T>()
        {
            return new T[0];
        }

        public static string Replace(string value)
        {
            return value.Replace("\\n", "\n");
        }

        #endregion
    }
}