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
using System.Reflection;
using System.Text;
using System.Xml;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 配置信息
    /// </summary>
    public class Settings
    {
        /// <summary>
        /// 服务器ID
        /// </summary>
        public static int ServerID { get; private set; }
        /// <summary>
        /// 主版本号
        /// </summary>
        public static int MajorVersion { get; private set; }
        /// <summary>
        /// 副版本号
        /// </summary>
        public static int MinorVersion { get; private set; }
        /// <summary>
        /// 修复版本号
        /// </summary>
        public static string RevisionInfo { get; private set; }
        /// <summary>
        /// 构建版本号
        /// </summary>
        public static string BuildInfo { get; private set; }
        /// <summary>
        /// 获取版本
        /// </summary>
        /// <returns></returns>
        public static string GetVersion() { return $"{MajorVersion}.{MinorVersion}.{(string.IsNullOrEmpty(RevisionInfo) ? "0" : RevisionInfo)}.{(string.IsNullOrEmpty(BuildInfo) ? "0" : BuildInfo)}"; }
        /// <summary>
        /// 脚本存放目录
        /// </summary>
        public static string CSScriptsPath { get; private set; }
        /// <summary>
        /// 编译后的程序集名称
        /// </summary>
        public static string AssemblyName { get; private set; }
        /// <summary>
        /// 服务器启动运行时,需要实现FantasyEngine.Network.Server.Runtime.CustomRuntime
        /// </summary>
        public static string MainClass { get; private set; }
        /// <summary>
        /// 是否是Debug模式
        /// </summary>
        public static bool IsDebug { get; private set; }
        /// <summary>
        /// 是否是发布模式
        /// </summary>
        public static bool IsRelease { get { return !IsDebug; } }
        /// <summary>
        /// 服务器配置队列
        /// </summary>
        public static IReadOnlyList<IHostSetting> HostSettings { get; private set; }
        /// <summary>
        /// Redis的配置信息
        /// </summary>
        public static IRedisSetting RedisSetting { get; private set; }
        /// <summary>
        /// 数据库连接信息
        /// </summary>
        public static IReadOnlyDictionary<string, IDatabaseSetting> DatabaseSettings { get; private set; }
        /// <summary>
        /// Redis Key 分隔符
        /// </summary>
        internal const char SPLITE_KEY = '_';
        /// <summary>
        /// 当数据(LoadAll)从db中全部拉取进来后，过多久释放
        /// </summary>
        public static int DataReleasePeriod { get; private set; }
        /// <summary>
        /// 每隔多少时间检测修改的数据并提交
        /// </summary>
        public static int DataCommitInterval { get; private set; }
        /// <summary>
        /// 线程锁的Timeout
        /// </summary>
        public static int LockerTimeout { get; private set; } = 100;
        /// <summary>
        /// 是否检查热数据并推到冷数据
        /// </summary>
        public static bool UseColdEntities { get; private set; }
        /// <summary>
        /// 热数据检查周期
        /// </summary>
        public static TimeSpan ColdEntitiesCheckoutInterval { get; private set; }
        /// <summary>
        /// 读取配置
        /// </summary>
        internal static void LoadFromFile()
        {
            string xmlFileName = "FantasyEngine.Network.Server.config";// Assembly.GetEntryAssembly().GetName().Name + ".config";
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.IgnoreComments = true;
            XmlDocument xml = new XmlDocument();
            using (XmlReader reader = XmlReader.Create(xmlFileName, settings))
            {
                xml.Load(reader);
            }
            IsDebug = xml.SelectSingleNode("/configuration/script/debug").GetValue(true);
            ServerID = xml.SelectSingleNode("/configuration/settings/server").GetValue(1);
            ConvertVersion(xml.SelectSingleNode("/configuration/settings/version").GetValue(null));
            CSScriptsPath = xml.SelectSingleNode("/configuration/script/cs-script-path").GetValue(null);
            AssemblyName = xml.SelectSingleNode("/configuration/script/assembly").GetValue(null);
            MainClass = xml.SelectSingleNode("/configuration/script/main-class-fullname").GetValue(null);
            //ModelNamespace = xml.SelectSingleNode("/configuration/script/model-namespace").GetValue("FantasyEngine.Network.Server.Model");
            DataReleasePeriod = xml.SelectSingleNode("/configuration/settings/data-release-period").GetValue(15000);
            DataCommitInterval = xml.SelectSingleNode("/configuration/settings/data-commit-interval").GetValue(1000);
            LockerTimeout = xml.SelectSingleNode("/configuration/settings/locker-timeout").GetValue(100);
            string checkoutInterval = xml.SelectSingleNode("/configuration/settings/data-checkout-interval").GetValue("30d");
            UseColdEntities = checkoutInterval != "0";
            if (UseColdEntities)
            {
                int interval;
                if (checkoutInterval.EndsWith("s") && int.TryParse(checkoutInterval.Substring(0, checkoutInterval.Length - 1), out interval))
                {
                    ColdEntitiesCheckoutInterval = TimeSpan.FromSeconds(interval);
                }
                else if (checkoutInterval.EndsWith("m") && int.TryParse(checkoutInterval.Substring(0, checkoutInterval.Length - 1), out interval))
                {
                    ColdEntitiesCheckoutInterval = TimeSpan.FromMinutes(interval);
                }
                else if (checkoutInterval.EndsWith("h") && int.TryParse(checkoutInterval.Substring(0, checkoutInterval.Length - 1), out interval))
                {
                    ColdEntitiesCheckoutInterval = TimeSpan.FromHours(interval);
                }
                else if (checkoutInterval.EndsWith("d") && int.TryParse(checkoutInterval.Substring(0, checkoutInterval.Length - 1), out interval))
                {
                    ColdEntitiesCheckoutInterval = TimeSpan.FromDays(interval);
                }
                else
                {
                    ColdEntitiesCheckoutInterval = TimeSpan.FromDays(30);
                }
            }

            //if (ModifiedCacheCount < 1) { ModifiedCacheCount = 3; }
            LoadHostSettings(xml);
            try
            {
                RedisSetting = new RedisSetting(xml.SelectSingleNode("/configuration/redis"));
            }
            catch { }

            try
            {
                XmlNode dbNodes = xml.SelectSingleNode("/configuration/connections");
                Dictionary<string, IDatabaseSetting> dbSettings = new Dictionary<string, IDatabaseSetting>();
                DatabaseSettings = dbSettings;
                foreach (XmlNode node in dbNodes)
                {
                    DatabaseSetting setting = new DatabaseSetting(node);
                    if (setting.Kind != EDatabase.Unknow)
                    {
                        dbSettings.Add(setting.ConnectKey, setting);
                    }
                    else
                    {
                        FEConsole.WriteError("Fail to load db setting!");
                    }
                }
            }
            catch
            {
                //DatabaseSettings = new Dictionary<string, IDatabaseSetting>();
            }
        }

        private static void LoadHostSettings(XmlDocument xml)
        {
            List<IHostSetting> settings = new List<IHostSetting>();
            HostSettings = settings;
            XmlNode node = xml.SelectSingleNode("/configuration/hosts");
            if (node == null)
            {
                return;
            }
            foreach (XmlNode child in node)
            {
                HostServerSetting setting = new HostServerSetting(child);
                if (setting.IsValid())
                {
                    settings.Add(setting);
                }
            }
        }

        private static void ConvertVersion(string version)
        {
            MajorVersion = MinorVersion = 0;
            RevisionInfo = BuildInfo = null;
            if (!string.IsNullOrEmpty(version))
            {
                string[] values = version.Split('.');
                MajorVersion = int.Parse(values[0]);
                if (values.Length > 1)
                {
                    MinorVersion = int.Parse(values[1]);
                }
                if (values.Length > 2)
                {
                    RevisionInfo = values[2];
                }
                if (values.Length > 3)
                {
                    BuildInfo = values[3];
                }
            }
        }

        public static string GetDefaultConnectKey()
        {
            if (DatabaseSettings == null || DatabaseSettings.Count == 0 || DatabaseSettings.ContainsKey("default"))
            {
                return "default";
            }
            return DatabaseSettings.Keys.First();
        }
    }
}
