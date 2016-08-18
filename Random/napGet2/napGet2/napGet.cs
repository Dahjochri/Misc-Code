using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace napGet2
{
    class napGet
    {
        private CookieContainer m_cookie = new CookieContainer();
        private string m_page = null;
        private string m_name = null;
        private string m_gid = null;
        private int m_id = -1;
        public string name
        {
            get
            {
                return m_name;
            }
        }
        public int id
        {
            get
            {
                return m_id;
            }
        }


        public void login()
        {
            //login
            int record = 0;
            string email="lol@lol.com";
            string fname = "hai";
            string lname = "bai";
            string user_agent = "Mozilla/5.0 (iPhone; U; XXXXX like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/241 Safari/419.3";

            string formUrl = "http://download.nap.edu/napguest.php";
            string formParams = string.Format("record_id={0}&page=/catalog.php?record_id={1}&email={2}&first_name={3}&last_name={4}&sector=Student", record, record, email, fname, lname);
            HttpWebRequest req = (HttpWebRequest) WebRequest.Create(formUrl);
            req.ContentType = "application/x-www-form-urlencoded";
            req.Method = "POST";
            req.UserAgent = user_agent;
            req.CookieContainer = m_cookie;
            byte[] bytes = Encoding.ASCII.GetBytes(formParams);
            req.ContentLength = bytes.Length;
            using (Stream os = req.GetRequestStream())
            {
                os.Write(bytes, 0, bytes.Length);
            }
            WebResponse resp = req.GetResponse();
            using (StreamReader sr = new StreamReader(resp.GetResponseStream()))
            {
                m_page = sr.ReadToEnd();
            }
            resp.Close();
        }
        private bool isFree()
        {
            Regex r = new Regex("No free downloads available for this title");
            Match m = r.Match(m_page);
            if (m.Success)
            {
                return false;
            }
            return true;
        }

        private string getName()
        {
            // Create a new Regex object.
            Regex r = new Regex("<div id=\"biblio_1\">\\s*?<h1>(.*)<\\/h1>\\s*?<h2>(.*)<\\/h2>");
            // Find a single match in the string.
            Match m = r.Match(m_page);
            if (m.Success)
            {
                m_name = String.Format("{0}{1}", m.Groups[1], m.Groups[2]);
                return m_name;
            }
            return "";
        }

        private string getGId()
        {
            Regex r = new Regex("<meta property=\"og:image\" content=\"http://images.nap.edu/images/minicov/(\\d*?).gif\"/>");
            Match m = r.Match(m_page);
            if (m.Success)
	        {
                m_gid = String.Format("{0}", m.Groups[1]);
                return m_gid;
	        }
            return "";
        }

        private void downloadGifs()
        {
        }

        private void makePdf()
        {
        }

        public string getBook(int id)
        {
            m_id = id;
            string getUrl = String.Format("http://www.nap.edu/catalog.php?record_id={0}", m_id);
            HttpWebRequest getRequest = (HttpWebRequest)WebRequest.Create(getUrl);
            getRequest.CookieContainer = m_cookie;
            WebResponse getResponse = getRequest.GetResponse();
            using (StreamReader sr = new StreamReader(getResponse.GetResponseStream()))
            {
                m_page = sr.ReadToEnd();
            }
            getResponse.Close();
            if (getName() == "")
            {
                //return false;
            }
            if (isFree())
            {
                //Download the book
                HttpWebRequest lala = (HttpWebRequest)WebRequest.Create(String.Format("http://download.nap.edu/cart/download.cgi?&record_id={0}&free=1", m_id));
                lala.CookieContainer = m_cookie;
                WebResponse lar = lala.GetResponse();
                using (StreamReader sr = new StreamReader(lar.GetResponseStream()))
                {
                    return sr.ReadToEnd();
                }
            }
            return "";
            //return true;
        }


    }
}
