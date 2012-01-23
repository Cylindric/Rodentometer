using System;
using System.IO;
using System.Net;
using System.Text;
using System.Xml;

namespace Rodentometer
{
    class Program
    {
        const string API_KEY = "vMZ2_PPD04LpatS9cohU0y_Exu5EwTXV-nTAyG5wx2fJJ81IcnlfJj0aLs2Q2xQeOqHTNjwiQuojQg49xAp2iA615kYR_rpkgQczOBldzpHYmt5PyDp643iHOMVc_VIKQ6JCsaaf1D-KSvBhp8Qg5Q";
        const string FEED = "45482";
        const string DATASTREAM = "test";

        static void Main(string[] args)
        {
            PachubeClient pachube = new PachubeClient(API_KEY, FEED);
            pachube.Send(DATASTREAM, 15);
        }

    }
}
