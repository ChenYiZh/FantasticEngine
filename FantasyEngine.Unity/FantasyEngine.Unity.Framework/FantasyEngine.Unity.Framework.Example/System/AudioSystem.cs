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
using System.IO;
using UnityEngine;
namespace FantasyEngine
{
    /// <summary>
    /// 音效管理类
    /// </summary>
    public partial class AudioSystem : SystemBasis<AudioSystem>
    {
        /// <summary>
        /// 音乐音量
        /// </summary>
        public float MusicVolume = 1.0f;
        /// <summary>
        /// 音效音量
        /// </summary>
        public float AudioVolume = 1.0f;
        /// <summary>
        /// 当需求是不能打断前一音效，需要对象池控制时的对象名称
        /// </summary>
        public const string AUDIO_NAME = "AUDIO[{0}]_{1}";
        /// <summary>
        /// 音乐播放控件
        /// </summary>
        public AudioSource MusicSource { get; private set; }
        /// <summary>
        /// 可以打断的音效播放控件
        /// </summary>
        public AudioSource AudioSource { get; private set; }
        /// <summary>
        /// 独立创建的音效列表，放完的音效会被删除
        /// </summary>
        public List<AudioSource> IndependentAudios { get; private set; }

        public AudioConfiguration AudioConfiguration { get; private set; }

        private bool _destroyOnAudioFinish = false;

        public void SetAudioVolume(float volume)
        {
            AudioSource.volume = volume;
            for (int i = IndependentAudios.Count - 1; i >= 0; i--)
            {
                var source = IndependentAudios[i];
                if (source)
                {
                    source.volume = volume;
                }
                else
                {
                    IndependentAudios.RemoveAt(i);
                }
            }
        }

        public override void Initialize()
        {
            base.Initialize();
            AudioConfiguration = AudioSettings.GetConfiguration();
            Reset();
            IndependentAudios = new List<AudioSource>();
            MusicSource = FEUtility.GetOrAddComponent<AudioSource>(Camera.main.gameObject);
            MusicSource.loop = true;
            MusicSource.playOnAwake = false;
            MusicSource.spatialBlend = 0;
            MusicSource.volume = MusicVolume;
            AudioSource = FEUtility.GetOrAddComponent<AudioSource>(GameRoot.Root.UIRoot.gameObject);
            AudioSource.loop = false;
            AudioSource.playOnAwake = false;
            AudioSource.spatialBlend = 0;
            AudioSource.volume = AudioVolume;
        }

        int _lastAudioId;

        public void PlayAudio(string fileName)
        {
            var source = AudioSource;
            if (FEUtility.NearBy(AudioVolume, 0) || source == null)
            {
                return;
            }

            GameRoot.Root.StartCoroutine(ResourceManager.Instance.LoadAsync<AudioClip>(
                GameDefines.Instance.ResourceAudioPath + fileName, clip => { PlayAudio(clip); }));
        }

        public void PlayAudio(AudioClip clip, bool destroyOnFinished = false)
        {
            var source = AudioSource;
            if (clip != null)
            {
                if (source.isPlaying)
                {
                    source.Stop();
                }

                if (source.clip != null)
                {
                    if (_destroyOnAudioFinish)
                    {
                        GameObject.Destroy(source.clip);
                        source.clip = null;
                    }
                }

                //lastAudioId = id;
                source.clip = clip;
                source.PlayOneShot(clip);
                _destroyOnAudioFinish = destroyOnFinished;
            }
        }

        public GameObject PlayAudioIndependent(string filename)
        {
            if (GameRoot.Root.Initialized)
            {
                GameObject obj = new GameObject();
                AudioSource source = FEUtility.GetOrAddComponent<AudioSource>(obj);
                IndependentAudios.Add(source);
                source.loop = false;
                source.playOnAwake = false;
                source.spatialBlend = 0;
                source.volume = AudioVolume;
                obj.name = Path.GetFileNameWithoutExtension(filename);
                GameRoot.Root.StartCoroutine(ResourceManager.Instance.LoadAsync<AudioClip>(
                    GameDefines.Instance.ResourceAudioPath + filename, clip =>
                    {
                        if (clip == null)
                        {
                            IndependentAudios.Remove(source);
                            GameObject.Destroy(obj);
                            return;
                        }

                        if (clip != null)
                        {
                            if (source.isPlaying)
                            {
                                source.Stop();
                            }

                            source.clip = clip;
                            source.PlayOneShot(clip);
                            GameRoot.Root.StartCoroutine(Wait2DelAudioSource(source));
                        }
                    }));
                return obj;
            }

            return null;
        }

        IEnumerator Wait2DelAudioSource(AudioSource source)
        {
            while (source && source.isPlaying)
            {
                yield return null;
            }

            if (source)
            {
                source.Stop();
                source.enabled = false;
                IndependentAudios.Remove(source);

                GameObject.Destroy(source.gameObject);
            }
        }

        public override void OnExit()
        {
            base.OnExit();
            for (int i = IndependentAudios.Count - 1; i >= 0; i--)
            {
                var source = IndependentAudios[i];
                if (source)
                {
                    source.Stop();
                    source.enabled = false;
                    GameObject.Destroy(source.gameObject);
                }
            }

            IndependentAudios.Clear();
        }

        bool lastIsPlaying = false;

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (lastIsPlaying && !AudioSource.isPlaying)
            {
                AudioSource.clip = null;
            }

            lastIsPlaying = AudioSource.isPlaying;
        }

        public void PlayMusic(string fileName)
        {
            GameRoot.Root.StartCoroutine(PlayMusicCoroutine(fileName));
        }

        private IEnumerator PlayMusicCoroutine(string fileName)
        {
            AudioClip music = null;
            yield return ResourceManager.Instance.LoadAsync<AudioClip>(GameDefines.Instance.ResourceMusicPath + fileName, clip =>
            {
                music = clip;
            });
            if (music != null)
            {
                MusicSource.clip = music;
                MusicSource.Play();
                MusicSource.volume = 0;
                //float time = 0;
                //while (time < GameDefines.MusicTransition)
                //{
                //    yield return null;
                //    MusicSource.volume = time / GameDefines.MusicTransition * GameDefines.Instance.MusicVolume;
                //    time += Time.deltaTime;
                //}
                MusicSource.volume = MusicVolume;
            }
        }

        public void PlayMusic(AudioClip music)
        {
            if (music != null)
            {
                MusicSource.clip = music;
                MusicSource.Play();
                MusicSource.volume = MusicVolume;
            }
        }

        public void StopMusic()
        {
            MusicSource.Stop();
            MusicSource.clip = null;
        }

        public bool IsMusicPlaying
        {
            get { return MusicSource.isPlaying; }
        }

        public bool Mute
        {
            get { return AudioSource.mute && MusicSource.mute; }
            set
            {
                AudioSource.mute = value;
                MusicSource.mute = value;
            }
        }

        public void Reset()
        {
            AudioSettings.Reset(AudioConfiguration);
            if (Camera.main.gameObject.GetComponent<AudioListener>())
            {
                GameObject.DestroyImmediate(Camera.main.gameObject.GetComponent<AudioListener>());
            }

            Camera.main.gameObject.AddComponent<AudioListener>();
        }
    }
}