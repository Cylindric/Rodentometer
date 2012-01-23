using System;
using System.IO;
using System.Net;
using System.Text;
using System.Diagnostics;
using System.Xml;

namespace Rodentometer
{
    public class PachubeClient
    {
        private const string BASE_URI = "http://api.pachube.com/v2/feeds/";

        private string _apiKey;
        private string _feedId;

        public PachubeClient(string apiKey, string feedId)
        {
            this._apiKey = apiKey;
            this._feedId = feedId;
        }

        public void Send(string datastream, int sample)
        {
            this.Send(datastream, sample.ToString());
        }

        public void Send(string datastream, string sample)
        {
            string uri = BASE_URI + this._feedId + "/datastreams/" + datastream + ".xml";
            WebClient wc = new WebClient();
            wc.Headers.Add("X-PachubeApiKey", this._apiKey);

            MemoryStream mem = new MemoryStream(wc.DownloadData(uri));
            XmlTextReader rdr = new XmlTextReader(mem);

            // Load the xml file into XmlDocument object.
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(rdr);
            }
            catch (XmlException e)
            {
                Console.WriteLine(e.Message);
            }

            // create some XML nodes to get the data out of the xml
            XmlNodeList data = xmlDoc.GetElementsByTagName("data");
            XmlNodeList items = data[0].ChildNodes;

            // now change the xml data value, to push it back out to the Pachube site
            items.Item(1).InnerXml = sample;

            // Now create StringWriter object to get data from xml document.
            StringWriter sw = new StringWriter();
            XmlTextWriter xw = new XmlTextWriter(sw);
            xmlDoc.WriteTo(xw);

            // upload it to pachube
            byte[] postArray = Encoding.ASCII.GetBytes(sw.ToString());
            wc.UploadData(uri, "PUT", postArray);
        }

    }
}