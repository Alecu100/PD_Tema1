using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Master {
    class Program {
        static void Main (string [] args) {
            ConsoleApplicationx1.Program p = new ConsoleApplicationx1.Program();

            //Process.Start ("../../../ConsoleApplication1/bin/Debug/ConsoleApplication1.exe");
            
            Process.Start (p.returnPath () + "\\ConsoleApplication1.exe", "rank 1");
            Process.Start (p.returnPath () + "\\ConsoleApplication1.exe", "rank 0");
        }
    }
}
