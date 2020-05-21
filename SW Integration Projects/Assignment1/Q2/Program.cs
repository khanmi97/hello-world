using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;

namespace SelfHostingClient
{
    class Program
    {
        static void Main(string[] args)
        {
            SelfHostingClientConsole prox = new SelfHostingClientConsole();
            int count = 0;
            string key = "";
            Console.WriteLine("Welcome to Number Guessing Game\n");
            Console.WriteLine("Set lower limit");
            string lower = Console.ReadLine();
            Console.WriteLine("Set upper limit");
            string upper = Console.ReadLine();

            int secNum = prox.SecretNumber(Int32.Parse(lower), Int32.Parse(upper)); // call method in service

            // for multiple attempts
            do
            {
                count++;
            
                Console.WriteLine("Make a guess");
                string guess = Console.ReadLine();

                Console.WriteLine("Attempts: " + count);
                Console.WriteLine("The number is " + prox.checkNumber(Int32.Parse(guess), secNum) + "\n");
                Console.WriteLine("Press 'r' to try again or press any other character to quit\n");
                key = Console.ReadLine();

            } while (String.Compare(key, "r") == 0);

            prox.Close();

        }
    }
}
