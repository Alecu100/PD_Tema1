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

int get_partial_pi_tag()
{
	return 666;
}

int get_num_sums_tag()
{
	return 667;
}

int main(int argc, char* argv[])
{
	int nr_sums, instance_id, nr_procs;
	double partial_pi, full_pi, h, sum, x, pi_buffer;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &instance_id);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_procs);

	while (true)
	{
		if (is_root_instance(instance_id))
		{
			cout << endl << "Please enter the number of individual sums or 0 to exit" << endl;
			cin >> nr_sums;

			for (int i = 1; i < nr_procs; i++)
			{
				MPI_Send(
					&nr_sums,
					1,
					MPI_INT,
					i,
					get_num_sums_tag(),
					MPI_COMM_WORLD);
			}
		}
		else
		{
			MPI_Recv(
				&nr_sums,
				1,
				MPI_INT,
				get_root_instance_id(),
				get_num_sums_tag(),
				MPI_COMM_WORLD,
				&status);
		}

		if (nr_sums == 0)
		{
			break;
		}

		h = 1.0 / static_cast<double>(nr_sums);
		sum = 0.0;

		for (int i = instance_id + 1; i <= nr_sums; i += nr_procs)
		{
			x = h * (static_cast<double>(i) - 0.5);
			sum += (4.0 / (1.0 + x * x));
		}

		partial_pi = h * sum;

		if (is_root_instance(instance_id))
		{
			full_pi = partial_pi;

			for (int i = 1; i < nr_procs; i++)
			{
				MPI_Recv(
					&pi_buffer,
					1,
					MPI_DOUBLE,
					i,
					get_partial_pi_tag(),
					MPI_COMM_WORLD, &status);

				full_pi += pi_buffer;
			}

			cout.precision(17);
			cout << endl << "Pi is " << full_pi << endl;
		}
		else
		{
			MPI_Send(
				&partial_pi,
				1,
				MPI_DOUBLE,
				get_root_instance_id(),
				get_partial_pi_tag(),
				MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}
