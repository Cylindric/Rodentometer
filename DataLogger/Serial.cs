using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace Rodentometer
{
    class Serial
    {
        private SerialPort _port = new SerialPort();;
        private string _baudRate = string.Empty;
        private string _parity = string.Empty;
        private string _stopBits = string.Empty;
        private string _dataBits = string.Empty;
        private string _portName = string.Empty;

        public Serial(string name)
        {
            _portName = name;
            _port.DataReceived += new SerialDataReceivedEventHandler(portDataReceived);
        }

        public bool OpenPort() 
        {
            try 
            {
                if (_port.IsOpen == true)
                {
                    _port.Close();
                }

                _port.BaudRate = int.Parse(_baudRate);
                _port.DataBits = int.Parse(_dataBits);
                _port.Parity = (Parity)Enum.Parse(typeof(Parity), _parity);
                _port.StopBits = (StopBits)Enum.Parse(typeof(StopBits), _stopBits);
                _port.PortName = _portName;

                _port.Open();
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        public void Write(string msg)
        {
            byte[] newMsg = HexToByte(msg);
            _port.Write(newMsg, 0, newMsg.Length);
        }

        private void portDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytes = _port.BytesToRead;
            byte[] comBuffer = new byte[bytes];
            _port.Read(comBuffer, 0, bytes);
        }

        private byte[] HexToByte(string msg)
        {
            msg = msg.Replace(" ", "");
            byte[] comBuffer = new byte[msg.Length / 2];
            for (int i = 0; i < msg.Length; i += 2)
            {
                comBuffer[i/2] = (byte)Convert.ToByte(msg.Substring(i, 2), 16);
            }
            return comBuffer;
        }
    }
}

