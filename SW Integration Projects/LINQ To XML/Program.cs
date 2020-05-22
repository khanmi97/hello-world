using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using System.IO;

namespace LinqToXml
{
    class Program
    {
        // Q 2.1: Definition of Course class
        class Course // 10 new courses are added in Courses.csv
        {
            public string subject;
            public Int32 courseCode;
            public string courseId;
            public string title;
            public string location;
            public string instructor;
        }

        // Q 2.4: Definition of Instructor class
        class Instructor // Instructions.csv file is added to the project
        {
            public string instructorName;
            public string officeNumber;
            public string emailAddress;
        }

        static void Main(string[] args)
        {
            string[] lines;
            var list = new List<string>();

            using (var textReader = new StreamReader("Courses.csv")) // reading from the .csv file
            {
                string line;
                while ((line = textReader.ReadLine()) != null)
                {
                    list.Add(line);
                }

                lines = list.ToArray();

            }

            Course[] courses = new Course[lines.Length - 1];

            int j = 0;
            for (int i = 1; i < lines.Length; i++)
            {
                string[] temp = lines[i].Split(',');
                courses[j] = new Course();
                courses[j].subject = temp[0];
                courses[j].courseCode = Convert.ToInt32(temp[1]);
                courses[j].courseId = temp[2];
                courses[j].title = temp[3];
                courses[j].location = temp[4];
                courses[j].instructor = temp[5];
                j++;
            }

            //Q 2.1: In-memory XML file of objects of Course class
            XDocument xmlDoc1 = new XDocument(
                new XDeclaration("1.0", "utf-8", "yes"),
                new XComment("Q 2.1: In-memory XML file of objects of Course class"),
                new XElement("Courses",
                    from course in courses
                    select new XElement("Course",
                        new XElement("Subject", course.subject),
                        new XElement("CourseCode", course.courseCode),
                        new XElement("ClassNbr", course.courseId),
                        new XElement("CourseTitle", course.title),
                        new XElement("Location", course.location),
                        new XElement("InstLastNm", course.instructor))));

            // Q 2.2: Save XML document
            xmlDoc1.Save(@"Courses.Xml");

            //Q 2.3a
            Console.WriteLine("Q  2.3a----------------------------------------------------------------------------------------------");

            IEnumerable<XElement> course1 =
                from c in XDocument.Load(@"Courses.Xml").Descendants("Course")
                where (string)c.Element("Subject") == "CPI" && (int)c.Element("CourseCode") >= 200
                orderby (string)c.Element("InstLastNm")
                select c;

            foreach(XElement c in course1)
            {
                Console.WriteLine("Course Title = {0}\nCourse Instructor = {1}", c.Element("CourseTitle").Value, c.Element("InstLastNm").Value);
                Console.WriteLine();
            }

            //Q 2.3b
            Console.WriteLine("Q  2.3b----------------------------------------------------------------------------------------------");

            var course2 =
                from c2 in XDocument.Load(@"Courses.Xml").Descendants("Course")
                group c2 by (string)c2.Element("CourseTitle") into g
                select new
                {
                    Subject = g.Key,

                    Title = from t in g.Elements("CourseCode")
                            select t
                };

            foreach(var g in course2)
            {
                if (g.Title.Count() >= 2)
                {
                    Console.WriteLine(g.Subject);
                }
            }

            // Q 2.4: Reading from Instructors.csv file

            var list2 = new List<string>();
            using (var textReader = new StreamReader("Instructors.csv"))
            {
                string line;
                while ((line = textReader.ReadLine()) != null)
                {
                    list2.Add(line);
                }
            }

            string[] lines2 = list2.ToArray();

            // Q 2.4: In-memory list of instructors
            List<Instructor> instructors = new List<Instructor>();

           
            for (int i = 1; i < lines2.Length; i++)
            {
                string[] temp2 = lines2[i].Split(',');
                Instructor ins = new Instructor();
                ins.instructorName = temp2[0];
                ins.officeNumber = temp2[1];
                ins.emailAddress = temp2[2];
                instructors.Add(ins);
            }

            // creating Instructors.Xml
            XDocument xmlDoc2 = new XDocument(
                new XDeclaration("1.0", "utf-8", "yes"),
                new XComment("XML document for instrcutors list"),
                new XElement("Instructors",
                    from instructor in instructors
                    select new XElement("Instructor",
                        new XElement("Name", instructor.instructorName),
                        new XElement("Office", instructor.officeNumber),
                        new XElement("Email", instructor.emailAddress))));

            xmlDoc2.Save(@"Instructors.Xml"); // saving the document

       

            // Q 2.4: Query
            Console.WriteLine("Q 2.4----------------------------------------------------------------------------------------------");
            XElement instElement = XElement.Load(@"Instructors.Xml");
            XElement courseElement = XElement.Load(@"Courses.Xml");


            var course3 =
                              from c3 in courseElement.Descendants("Course")
                              join ins in instElement.Descendants("Instructor") on (string)c3.Element("InstLastNm").Value equals (string)ins.Element("Name").Value
                              orderby (int)c3.Element("CourseCode")
                              select new
                              {
                                  CourseCode = (int)c3.Element("CourseCode"),
                                  Subject = (string)c3.Element("Subject"),
                                  Email = (string)ins.Element("Email")
                              };

            foreach (var q in course3)
            {
                if (q.CourseCode >= 200)
                {
                    Console.WriteLine("Class: {0} {1}\nInstructor's Email: {2}", q.Subject, q.CourseCode, q.Email);
                }
            }
        }
    }
}
