// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 license.

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using mDNSUWP;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace mDNSTest
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private HomeKit homeKit;

        public MainPage()
        {
            this.InitializeComponent();
        }

        private void btnRelease_Click(object sender, RoutedEventArgs e)
        {
            homeKit.Close();
            txtRetCode.Text = "";
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Dictionary<string, string> metadata = new Dictionary<string, string>();

            metadata.Add("c#", "1");
            metadata.Add("ff", "0");
            metadata.Add("id", idTextBox.Text);
            metadata.Add("md", "Microsoft1,1");
            metadata.Add("s#", "1");
            metadata.Add("sf", "1");
            metadata.Add("ci", "5");

            homeKit = new HomeKit();

            int result = homeKit.Register("LHFridge2", "_http._tcp", "local", 1067, metadata);

            Debug.WriteLine("Register returned {0}.", result);

            txtRetCode.Text = result.ToString();
        }
    }
}
