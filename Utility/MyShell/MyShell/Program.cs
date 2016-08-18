using System;                       //For the lulz!
using System.IO;                    // For directory!
using System.Collections.Generic;   // For Lists!
using System.Security.Principal;    // For username!

namespace MyShell
{
    class Program
    {
        static string last_dir;

        static void Main(string[] args)
        {
            List<string> input;
            string command;

            Console.Clear();

            while (true)
            {
                Console.Write(get_prefix());
                input = new List<string> (Console.ReadLine().Split(' '));
                command = input[0];
                input.RemoveAt(0);


                switch (command)
                {
                    case "clear":
                        Console.Clear();
                        break;

                    case "cd":
                        foreach (string item in input)
                        {
                            Console.Write(item);
                        }
                        change_dir(input);
                        break;

                    case "ls":
                        //intentional fallthrough
                    case "list":
                        list(input);
                        break;

                    case "cat":
                        cat(input);
                        break;

                    case "cp":
                        //Intentional Fallthrough
                    case "copy":
                        copy(input);
                        break;

                    case "del":
                        //Intentional Fallthrough
                    case "rm":
                        //Intentional Fallthrough
                    case "delete":
                        //Intentional Fallthrough
                    case "remove":
                        delete(input);
                        break;

                    case "exit":
                        //Intentional Fallthrough
                    case "quit":
                        Environment.Exit(0);
                        break;

                    default:
                        if (command.Trim()!="")
                        {
                            Console.WriteLine("Command \"{0}\" not understood", command);
                        }
                        break;
                }
            }
        }

        static string get_prefix()
        {
            string tmptext;
            tmptext = SystemInfo.Username
            + "@" + SystemInfo.Hostname
            + "|" + Directory.GetCurrentDirectory() + ">";
            return tmptext;
        }

        static void change_dir(List<string> input)
        {
            try
            {
                if (input.Count == 1 && input[1] == "--last" && last_dir != null)
                {
                    Directory.SetCurrentDirectory(last_dir);
                }
                else
                {
                    last_dir = Directory.GetCurrentDirectory();
                    Directory.SetCurrentDirectory(input[0]);
                }
            }
            catch(System.Exception e)
            {
                Console.WriteLine(e);
                throw e;
            }
        }

        static void list(List<string> input)
        {
            string[] files = Directory.GetFiles("./");
            string[] dirs = Directory.GetDirectories("./");
            foreach (string dir in dirs)
            {
                Console.WriteLine(dir);
            }
            foreach (string file in files)
            {
                Console.WriteLine(file);
            }
        }

        static void cat(List<string> input)
        {
            StreamReader sr = new StreamReader(input[0]);
            Console.Write(sr.ReadToEnd());
            sr.Close();
        }

        static void copy(List<string> input)
        {
            if (input.Count != 2)
            {
                Console.WriteLine("Fayul!");
                return;
            }

            if (File.Exists(input[1]))
            {
                if (GetAnswer(String.Format("Overwrite {0}?", input[1])))
                {
                    File.Delete(input[1]);
                }
                else
                {
                    return;
                }
            }
            File.Copy(input[0], input[1]);
            Console.WriteLine("{0} copied to {1}.", input[0], input[1]);
        }

        static void delete(List<string> input)
        {
            bool confirmation = true;
            string fname = input[0];
            if (input.Count == 2)
            {
                fname = input[1];
                switch (input[0])
                {
                    case "/y":
                        //Intentional Fallthrough
                    case "/Y":
                        confirmation = false;
                        break;
                }
            }
            

            if (confirmation)
            {
                if (!GetAnswer(String.Format("Do you REALLY want to delete {0}?",fname)))
                {
                    return;
                }
            }
            File.Delete(fname);
        }

        static bool GetAnswer(string info)
        {
            while (true)
            {
                Console.Write(info + " (Y/N):");
                char c = Console.ReadKey().KeyChar;
                Console.Write(Console.Out.NewLine);
                switch (c)
                {
                    case 'y':
                    case 'Y':
                        return true;
                    case 'n':
                    case 'N':
                        return false;
                }
            }
        }

        }
}
