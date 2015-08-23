using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Threading;
using Interop;

namespace RLTools
{
    /// <summary>
    /// Interaction logic for Loading.xaml
    /// </summary>
    public partial class Loading : Window
    {
        public Loading()
        {
            InitializeComponent();
        }

        private Config _Config;
        private Interoperation _Interop;

        private void Start()
        {
            MainWindow mw = new MainWindow();
            mw.Config = _Config;
            mw.Interop = _Interop;
            mw.Show();
            this.Close();
        }

        private void LoadTinyFS()
        {
            try
            {
                _Interop = new Interoperation();
                //int ret = _Interop.LoadTinyFSAssemble(_Config.TinyFSFile);
                //if (ret == 0)
                //{
                //    MessageBox.Show("Load TinyFS.dll Failed, RLTools will shutdown!",
                //        "Fatal Error!", MessageBoxButton.OK, MessageBoxImage.Error);
                //    this.Close();
                //}
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public enum ProgressState
        {
            LoadingStart,
            LoadingConfig,
            CreateConfig,
            LoadingTinyFS,
            End
        }

        public delegate void UpdateUIStateDelegate(string info, ProgressState step);
        private void UpdateProgressLabel(string info, ProgressState step)
        {
            ProgressTextBlock.Text = info;

            switch (step)
            {
                case ProgressState.End:
                    Start();
                    break;
                case ProgressState.LoadingTinyFS:
                    LoadTinyFS();
                    break;
            }
        }

        private void LoadingThread()
        {
            ProgressTextBlock.Dispatcher.Invoke(new UpdateUIStateDelegate(UpdateProgressLabel),
                "Loading Config File(config.xml).", ProgressState.LoadingStart);
            Thread.Sleep(1000);

            Config con = Config.LoadConfig(@"config.xml");
            _Config = con;
            if (con.State == ConfigState.NotExisted)
            {
                ProgressTextBlock.Dispatcher.Invoke(new UpdateUIStateDelegate(UpdateProgressLabel),
                    "配置文件(config.xml)不存在。", ProgressState.LoadingStart);
                Thread.Sleep(100);

                ProgressTextBlock.Dispatcher.Invoke(new UpdateUIStateDelegate(UpdateProgressLabel),
                    "创建配置文件(config.xml)。", ProgressState.CreateConfig);
                Thread.Sleep(1000);

                con.Save();
            }

            ProgressTextBlock.Dispatcher.Invoke(new UpdateUIStateDelegate(UpdateProgressLabel),
                "Loading TinyFS.", ProgressState.LoadingTinyFS);
            Thread.Sleep(500);

            ProgressTextBlock.Dispatcher.Invoke(new UpdateUIStateDelegate(UpdateProgressLabel),
                "启动中。", ProgressState.End);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                Thread thread = new Thread(new ThreadStart(LoadingThread));
                thread.Start();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
