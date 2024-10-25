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
using System.Linq;
using System.Threading;
using FantasyEngine.Log;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
namespace FantasyEngine
{
    /// <summary>
    /// 核心类
    /// </summary>
    public class GameRoot : MonoBehaviour
    {
        /// <summary>
        /// 配置信息
        /// </summary>
        [SerializeField] private GameDefines _gameDefines;

        /// <summary>
        /// 版本信息
        /// </summary>
        [SerializeField] private VersionInfo _version;

        public static int ThreadId { get; private set; }

        /// <summary>
        /// 配置信息
        /// </summary>
        public GameDefines GameDefines
        {
            get { return _gameDefines; }
        }

        /// <summary>
        /// 版本信息
        /// </summary>
        public VersionInfo Version
        {
            get { return _version; }
        }

        public Scene RootScene { get; private set; }

        public static EGameState GameState { get; private set; }

        public bool Initialized
        {
            get { return GameState > EGameState.Ready; }
        }

        public HashSet<SystemBasis> Systems { get; private set; }

        /// <summary>
        /// UI对象
        /// </summary>
        public UIRoot UIRoot { get; private set; }

        public class EventParam_Message : EventParam
        {
            public string message;
        }

        /// <summary>
        /// 准备完成，开始启动
        /// </summary>
        private void StartGame()
        {
            FConsole.Write("GameRoot.StartGame");
        }

        private void Awake()
        {
            _gameDefines = GameObject.Instantiate(_gameDefines);

            ThreadId = Thread.CurrentThread.ManagedThreadId;
            _instance = this;

            GameDefines.Initialize();

            GameObject nativeSDK = new GameObject("NativeSDK");

            FConsole.RegistLogger(Logger.Instance);
            FConsole.RegistLogger(LoggerToast.Instance);
            FConsole.RegistLogger(LogFile.Instance);
            UIRoot = FEUtility.GetOrAddComponent<UIRoot>(transform.Find("UIRoot"));

            //gameObject.AddComponent<Console>();
            Application.runInBackground = true;
            Screen.sleepTimeout = SleepTimeout.NeverSleep; //设置屏幕常亮
            Application.lowMemory += OnLowMemory;
            RootScene = gameObject.scene;

            DontDestroyOnLoad(this);

            RectTransform rect = UIRoot.GetComponent<RectTransform>();
            GameDefines.Instance.uiWidth = rect.rect.width;
            GameDefines.Instance.uiHeight = rect.rect.height;
            GameDefines.Instance.uiAbsWidth = rect.rect.width / rect.localScale.x;
            GameDefines.Instance.uiAbsHeight = rect.rect.height / rect.localScale.y;

            ResourceManager.Instance.Release();

            Systems = new HashSet<SystemBasis>();
            GlobalConfig.RegistSystem(Systems);
            foreach (var system in Systems)
            {
                try
                {
                    system.Initialize();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            OnCreate();
        }

        private void OnCreate()
        {
            //EventSystem.Instance.RegistEvent(EEvent.Login, OnLogin);
            //EventSystem.Instance.RegistEvent(EEvent.Logout, OnLogout);
            //EventSystem.Instance.RegistEvent(EEvent.BeginPlay, Play);
            //EventSystem.Instance.RegistEvent(EEvent.GameFinish, BeginExit);

            //EventSystem.Instance.RegistEvent(EEvent.GamePause, OnPause);
            //EventSystem.Instance.RegistEvent(EEvent.GameContinue, OnContinue);


            Ready();

        }

        public void Ready(EventParam param = null)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.Prepare();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            TableManager.Instance.Load(new TableLoaderConfig()
            {
                TablePath = GameDefines.Instance.ResourceTablePath,
                BasePath = GameDefines.Instance.WritableTablePath,
                IdxUrl = GameDefines.Instance.GetOnlineIdxUrl(),
                TablesUrl = GameDefines.Instance.GetOnlineTablesUrl(),
                //UseLocalTable = GameDefines.Instance.UseLocalTable,
                UseLocalTable = true,
                Shift = 2,
            });

            GameState = EGameState.Prepare;
        }

        private void BeforeOnReady()
        {
            StopAllCoroutines();
            //SceneManager.SetActiveScene(RootScene);
            // #if !UNITY_EDITOR
            //         if (Application.targetFrameRate != GameSettings.Instance.TargetFrameRate)
            //         {
            //             Application.targetFrameRate = 60;//GameSettings.Instance.TargetFrameRate;
            //         }
            // #endif
#if UNITY_IOS
        ResourceManager.Instance.Release();
#endif
            // if (!GameSystem.Instance.TryAgain)
            // {
            //     EventSystem.Instance.Send(Events.LoadingProgress, new TipsLoadingProgress() { Progress = 1 });
            // }
        }

        private void CheckReady()
        {
            if (GameState != EGameState.Prepare &&
                GameState != EGameState.Stopped)
            {
                return;
            }

            bool ready = true;
            foreach (var system in Systems)
            {
                if (!system.Ready)
                {
                    ready = false;
                }
            }

            if (!ready)
            {
                return;
            }

            GameState = EGameState.Ready;

            BeforeOnReady();

            foreach (var system in Systems)
            {
                try
                {
                    system.OnReady();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            GameState = EGameState.Playing;

            Play();
        }

        public void Play(EventParam data = null)
        {
            foreach (var system in Systems)
            {
                try
                {
                    //system.BeginPlay(param);
                    system.BeginPlay();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            StartGame();
        }

        private static GameRoot _instance = null;

        public static GameRoot Root
        {
            get { return _instance; }
        }

        private void OnLogin(EventParam param)
        {
            Systems.Add(PlayerData.Instance);
            foreach (var system in Systems)
            {
                try
                {
                    system.OnLogin();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnLogout(EventParam param)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnLogout();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
            Systems.Remove(PlayerData.Instance);
        }

        private void OnPlay(EventParam param)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnPlay();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnPause(EventParam param)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnPause();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnContinue(EventParam param)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnContinue();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void BeginExit(EventParam param)
        {
            OnExit();
        }

        private void OnExit()
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnExit();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            // Proxy.OnExit();
        }


        //float lastTime;

        private void Update()
        {
            //Debug.LogError("Unity Is Running...");
            CheckReady();

            foreach (var system in Systems)
            {
                try
                {
                    if (Initialized || system.UpdateBeforeInitialze)
                    {
                        system.OnUpdate();
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }

            //if (Input.GetKey(KeyCode.A))
            //{
            //    StartOfflineClick();
            //}
        }

        private void LateUpdate()
        {
            foreach (var system in Systems)
            {
                try
                {
                    if (Initialized || system.UpdateBeforeInitialze)
                    {
                        system.OnLateUpdate();
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void FixedUpdate()
        {
            foreach (var system in Systems)
            {
                try
                {
                    if (Initialized || system.UpdateBeforeInitialze)
                    {
                        system.OnFixedUpdate();
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnApplicationQuit()
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnApplicationQuit();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnApplicationFocus(bool focus)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnApplicationFocus(focus);
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void OnApplicationPause(bool pause)
        {
            foreach (var system in Systems)
            {
                try
                {
                    system.OnApplicationPause(pause);
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        // /// <summary>
        // /// 掉网了
        // /// </summary>
        // private bool _lost = false;
        //
        // /// <summary>
        // /// 掉网时长
        // /// </summary>
        // private int _lostSeconds = 0;
        //
        // /// <summary>
        // /// 重连次数
        // /// </summary>
        // private int _tryConnectCount = 0;
        //
        // private void CheckConnection()
        // {
        //     if (_lost && GameSystem.IsOnline)
        //     {
        //         if (Net.Instance.Connected)
        //         {
        //             _lost = false;
        //             return;
        //         }
        //
        //         if (_tryConnectCount >= 5)
        //         {
        //             var msgbox = (Tips_MessageBox)Tips.Root.Panels[ETips.Tips_MessageBox];
        //             if (msgbox.Visible)
        //             {
        //                 return;
        //             }
        //
        //             int count = msgbox.Count;
        //             count++;
        //             Util.ShowMessageBox(EMsgType.YesNo, 30145, () =>
        //             {
        //                 if (!Net.Instance.Connected)
        //                 {
        //                     GameSystem.Instance.QuickReconnect();
        //                     Proxy.Instance.OnReconnect();
        //                 }
        //             }, 30146, () =>
        //             {
        //                 GameSystem.Instance.TryAgain = false;
        //                 GameSystem.GameMode.Exit();
        //             }, 30137);
        //             StartCoroutine(Delay2CloseMsg(count));
        //             return;
        //         }
        //
        //         _lostSeconds--;
        //         if (_lostSeconds <= 0)
        //         {
        //             _lostSeconds = 5;
        //             _tryConnectCount++;
        //             GameSystem.Instance.QuickReconnect();
        //             return;
        //         }
        //     }
        // }
        //
        // IEnumerator Delay2CloseMsg(int vailedCount)
        // {
        //     float time = 0;
        //     int maxTime = 5;
        //     var tipsPanel = ((Tips_MessageBox)Tips.Root.Panels[ETips.Tips_MessageBox]);
        //     while (time < maxTime && tipsPanel.Count <= vailedCount && tipsPanel.Visible)
        //     {
        //         UnityEngine.UI.Text yesTxt = tipsPanel.YesTxt;
        //         int deltaSeconds = Mathf.RoundToInt(maxTime - time);
        //         yesTxt.text = TableLanguage.Instance.GetText(30146).Replace(" ", "\u00A0") + $"\u00A0({deltaSeconds})";
        //         if (!Tips.Root.Panels[ETips.Tips_MessageBox].Visible)
        //         {
        //             yield break;
        //         }
        //
        //         yield return null;
        //         time += Time.deltaTime;
        //     }
        //
        //     if (tipsPanel.Count <= vailedCount && tipsPanel.Visible)
        //     {
        //         tipsPanel.Hide();
        //         if (!Net.Instance.Connected)
        //         {
        //             GameSystem.Instance.QuickReconnect();
        //             Proxy.Instance.OnReconnect();
        //         }
        //     }
        // }
        //
        // private void OnDisconnected(EventParam param)
        // {
        //     if (GameSystem.IsOnline)
        //     {
        //         if (!_lost)
        //         {
        //             _lostSeconds = 5;
        //             _tryConnectCount = 0;
        //         }
        //
        //         _lost = true;
        //     }
        //     else
        //     {
        //         _lost = false;
        //     }
        // }

        private void OnDestroy()
        {
            //EventSystem.Instance.UnregistEvent(EEvent.Login, OnLogin);
            //EventSystem.Instance.UnregistEvent(EEvent.Logout, OnLogout);
            //EventSystem.Instance.UnregistEvent(EEvent.BeginPlay, Play);
            //EventSystem.Instance.UnregistEvent(EEvent.GameFinish, BeginExit);

            //EventSystem.Instance.UnregistEvent(EEvent.GamePause, OnPause);
            //EventSystem.Instance.UnregistEvent(EEvent.GameContinue, OnContinue);

            //EventSystem.Instance.UnregistEvent(Events.Disconnected, OnDisconnected);

            FConsole.RemoveLogger(Logger.Instance);
            FConsole.RemoveLogger(LoggerToast.Instance);
            FConsole.RemoveLogger(LogFile.Instance);
        }

        private void OnLowMemory()
        {
            TextureSystem.Instance.OnLowMemory();
            ResourceManager.Instance.Release();
        }
    }
}