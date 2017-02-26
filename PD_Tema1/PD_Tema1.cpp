// PD_Tema1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include "mpi.h"
#include <iostream>

using namespace std;

bool is_root_instance(int instance_id)
{
	return instance_id == 0;
}

int get_root_instance_id()
{
	return 0;
}

int main(int argc, char* argv[])
{
	int nr_sums, instance_id, nr_procs, i;
	double partial_pi, full_pi, h, sum, x;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &instance_id);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_procs);

	while (true)
	{
		if (is_root_instance(instance_id))
		{
			cout << endl << "Please enter the number of individual sums or 0 to exit" << endl;
			cin >> nr_sums;
		}

		if (nr_sums == 0)
		{
			break;
		}

		MPI_Bcast(&nr_sums, 1, MPI_INT, get_root_instance_id(), MPI_COMM_WORLD);

		h = 1.0 / static_cast<double>(nr_sums);
		sum = 0.0;

		for (i = instance_id + 1; i <= nr_sums; i += nr_procs)
		{
			x = h * (static_cast<double>(i) - 0.5);
			sum += (4.0 / (1.0 + x * x));
		}

		partial_pi = h * sum;

		MPI_Reduce(&partial_pi, &full_pi, 1, MPI_DOUBLE, MPI_SUM, get_root_instance_id(), MPI_COMM_WORLD);

		if (is_root_instance(instance_id))
		{
			cout << endl << "Pi is " << full_pi << endl;
		}
	}

	MPI_Finalize();
	return 0;
}
