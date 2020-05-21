using System;
using System.Linq;
using System.Collections.Generic;
using System.IO;

namespace LINQ_Query
{
    // Q 1.1: Definition of Course class
    class Course // 10 new courses are added in Courses.csv
    {
        public string subject;
        public Int32 courseCode;
        public string courseId;
        public string title;
        public string location;
        public string instructor;
    }

    // Q 1.4: Definition of Instructor class
    class Instructor // Instructions.csv file is added to the project
    {
        public string instructorName;
        public string officeNumber;
        public string emailAddress;
    }

    class Program
    {
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
            // Q 1.1: in-memory array of objects of the Course class
            Course[] courses = new Course[lines.Length - 1];

            int j = 0;
            for (int i = 1; i < lines.Length; i++)
            {
                string[] temp = lines[i].Split(",");
                courses[j] = new Course();
                courses[j].subject = temp[0];
                courses[j].courseCode = Convert.ToInt32(temp[1]);
                courses[j].courseId = temp[2];
                courses[j].title = temp[3];
                courses[j].location = temp[4];
                courses[j].instructor = temp[5];
                j++;
            }

            //Q 1.2a 
            Console.WriteLine("Question 1.2a-------------------------------------------------------------------------------------------------------");
            IEnumerable<Course> myQuery =
                from c in courses
                where c.subject.CompareTo("IEE") == 0 && c.courseCode >= 300
                orderby c.instructor
                select c;
            foreach (Course item in myQuery)
            {
                Console.WriteLine("Course Title = {0}\nCourse Instructor = {1}", item.title, item.instructor);
                Console.WriteLine();
            }

            //Q 1.2b
            Console.WriteLine("Question 1.2b-------------------------------------------------------------------------------------------------------");
            var myQuery2 =
                from c1 in courses
                group c1 by c1.subject into g
                select new { Subject = g.Key, Codes = g };
            foreach (var g in myQuery2)
            {
                int i = 0;
                foreach (var c1 in g.Codes)
                {
                    i++;
                }
                if (i >= 2)
                {
                    Console.WriteLine("Subject {0} that has 2 or more of the course IDs", g.Subject);
                }
            }

            // Q 1.4: Reading from Instructors.csv file

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

            // Q 1.4: In-memory list of instructors
            List<Instructor> instructors = new List<Instructor>();

            int k = 0;
            for (int i = 1; i < lines2.Length; i++)
            {
                string[] temp2 = lines2[i].Split(",");
                Instructor ins = new Instructor();
                ins.instructorName = temp2[0];
                ins.officeNumber = temp2[1];
                ins.emailAddress = temp2[2];
                instructors.Add(ins);
            }

            // Q 1.5
            Console.WriteLine("Q 1.5----------------------------------------------------------------------------------------------------");

            var myquery3 =
                from course in courses
                join instructor in instructors on course.instructor equals instructor.instructorName
                orderby course.courseCode
                select new { CourseCode = course.courseCode, Subject = course.subject, Name = instructor.instructorName, Email = instructor.emailAddress };
            foreach (var q in myquery3)
            {
                if (q.CourseCode >= 200)
                {
                    Console.WriteLine("Class: {0} {1}\nInstructor's Email: {2}", q.Subject, q.CourseCode, q.Email);
                }
            }
        }
    }
}
