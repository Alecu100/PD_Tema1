using MPI;
using System;

namespace ConsoleApplicationx1 {
    public class Program {
        public static void Main (string [] args) {
            using (new MPI.Environment (ref args)) {
                Intracommunicator comm = Communicator.world;

                int nr_sums = 0, nr_procs = Communicator.world.Size;
                double partial_pi, full_pi, h, sum, x, pi_buffer;

                while (true) {
                    if (is_root_instance (comm.Rank)) {
                        string str = string.Empty;
                        Console.WriteLine ("Please enter the number of individual sums or 0 to exit");
                        str = Console.ReadLine ();
                        nr_sums = int.Parse (str);

                        for (int i = 1; i < nr_procs; i++) {
                            comm.Send (nr_sums, 1, get_num_sums_tag ());
                        }
                    } else {
                        nr_sums = comm.Receive <int> (Communicator.anySource, get_num_sums_tag ());
                    }

                    if (nr_sums == 0) {
                        break;
                    }

                    h = 1.0 / (double) (nr_sums);
                    sum = 0.0;

                    for (int i = comm.Rank + 1; i <= nr_sums; i += nr_procs) {
                        x = h * ((double) (i) - 0.5);
                        sum += (4.0 / (1.0 + x * x));
                    }

                    partial_pi = h * sum;

                    if (is_root_instance (comm.Rank)) {
                        full_pi = partial_pi;

                        for (int i = 1; i < nr_procs; i++) {
                            pi_buffer = comm.Receive <double> (Communicator.anySource, get_partial_pi_tag ());
                            full_pi += pi_buffer;
                        }

                        Console.WriteLine ("Pi is " + full_pi);
                    } else {
                        comm.Send <double> (partial_pi, 1, get_partial_pi_tag ());
                    }
                }
            }
        }

        static bool is_root_instance (int instance_id) {
            return instance_id == 0;
        }

        static int get_root_instance_id () {
            return 0;
        }

        static int get_partial_pi_tag () {
            return 666;
        }

        static int get_num_sums_tag () {
            return 667;
        }
    }
}
