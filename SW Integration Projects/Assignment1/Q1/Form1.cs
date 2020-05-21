using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    
    public partial class Form1 : Form
    {
        ServiceReference1.Service1Client proxy = new ServiceReference1.Service1Client();
        int secNum = 0;
        int count = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            secNum = proxy.SecretNumber(Int32.Parse(textBox1.Text), Int32.Parse(textBox2.Text));
        }

        private void button2_Click(object sender, EventArgs e)
        {
            count++;
            label5.Text = count.ToString();
            label6.Text = proxy.checkNumber(Int32.Parse(textBox3.Text), secNum);
        }
    }
}
