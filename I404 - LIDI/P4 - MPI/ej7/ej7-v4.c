// IMPLEMENTACIÓN PARALELA

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mpi.h>
#include <omp.h>

#define COORDINATOR 0

// ------------------ MODULARIZACION -------------------

void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs)
{
  int i, j, k;

  for (i = 0; i < bs; i++){
    for (j = 0; j < bs; j++){
      for  (k = 0; k < bs; k++){
        cblk[i*n + j] += ablk[i*n + k] * bblk[j*n + k];
      }
    }
  }
}

void blksum(double *ablk, double *bblk, double *cblk, double *resblk, int n, int bs)
{
  int i, j, k;

  for (i = 0; i < bs; i++){
    for (j = 0; j < bs; j++){
      resblk[i*n+j] = ablk[i*n+j] + bblk[i*n+j] + cblk[i*n+j];
    }
  }
}

void matmul(double* a, double* b, double* c, int stripSize, int n, int bs){
  int i,j,k;

  // paralelizamos con 8 hilos
  #pragma omp for schedule(static) nowait
	for (i=0; i<stripSize; i+=bs){
	  for (j=0; j<n; j+=bs){
		  for (k=0; k<n; k+=bs){
		  	blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n, bs);
		  }
	  }
	}
}

void matsum(double *a, double *b, double *c, double *res, int stripSize, int n, int bs){
  int i,j;

  // paralelizamos con 8 hilos
  #pragma omp for schedule(static)
  for (i=0; i<stripSize; i+=bs) {
		for (j=0; j<n; j+=bs) {
      blksum(&a[i*n + j], &b[i*n + j], &c[i*n + j], &res[i*n+j], n, bs);
		}
	}
}

// ------------------------------------------------

// N se recibe por parámetro
int main(int argc, char* argv[]){
  int i, j, k, numProcs, rank, n, bs, stripSize, check=1;
	double *a, *b, *c, *d, *e, *f, *r1, *r2, *r3, *rf;
	double commTimes[4], commTime, totalTime;
  int provided;

  MPI_Status status;

	/* Leer argumentos */
  if (argc != 3){
    printf("cantidad incorrecta de parametros");
    exit(1);
  }

  n = atoi(argv[1]);
  bs = atoi(argv[2]);

  if (n <= 0 || bs <= 0){
    printf("\nUsar: %s size bs \n  size: Dimension de las matrices\n  bs: Dimension de bloque\n", argv[0]);
		exit(2);
  }
  
  // control de N y bs
  if (n % bs != 0){
    printf("Usar N múltiplo de bs\n");
    exit(2);
  }

  // inicialización de MPI
	MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
	MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  // chequear que N sea multiplo de numProcs
  if (n % numProcs != 0) {
		printf("N debe ser multiplo del numero de procesos.\n");
		exit(1);
	}

  // calcular porcion de cada worker
	stripSize = n / numProcs;

  // el master es quien reparte -> n*n
  // los workers reciben porcion -> stripSize*n
  int cantFilas = rank == COORDINATOR ? n : stripSize;

  // Reservar memoria para las matrices operando
  a = (double*) malloc(sizeof(double)*cantFilas*n);   // A -> scatter
  b = (double*) malloc(sizeof(double)*n*n);           // B -> bcast
  c = (double*) malloc(sizeof(double)*cantFilas*n);   // C -> scatter
  d = (double*) malloc(sizeof(double)*n*n);           // D -> bcast
  e = (double*) malloc(sizeof(double)*cantFilas*n);   // E -> scatter
  f = (double*) malloc(sizeof(double)*n*n);           // F -> bcast

  // Reservar memoria para las matrices temporales
  r1 = (double*) malloc(sizeof(double)*stripSize*n);
  r2 = (double*) malloc(sizeof(double)*stripSize*n);
  r3 = (double*) malloc(sizeof(double)*stripSize*n);

  // Reservar memoria para la matriz resultado
  rf = (double*) malloc(sizeof(double)*cantFilas*n);  // rf -> gather

  // Inicializar datos de operandos (solo el master)
  if (rank == COORDINATOR) {
		for (i=0; i<n ; i++)
			for (j=0; j<n ; j++){
        a[i*n+j] = 1;			// por filas
        b[i*n+j] = 1;
        c[i*n+j] = 1;
        d[i*n+j] = 1;
        e[i*n+j] = 1;
        f[i*n+j] = 1;
      }
	}

  MPI_Barrier(MPI_COMM_WORLD);

  // tiempo 0 = arranque de las comunicaciones
	if (rank == COORDINATOR) commTimes[0] = MPI_Wtime();

  // ------------------------- TRABAJO ---------------------------------

  // primero repartir el primer operando de las 3 mult
  MPI_Scatter(a, stripSize*n, MPI_DOUBLE, a, stripSize*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
  MPI_Scatter(c, stripSize*n, MPI_DOUBLE, c, stripSize*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
  MPI_Scatter(e, stripSize*n, MPI_DOUBLE, e, stripSize*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);

  // copiar segundo operando de las 3 mult
  MPI_Bcast(b, n*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
  MPI_Bcast(d, n*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
  MPI_Bcast(f, n*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);

  // computo paralelo
  if (rank == COORDINATOR) commTimes[1] = MPI_Wtime();
  omp_set_nested(1);

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      matmul(a, b, r1, stripSize, n, bs);

      #pragma omp section
      
    }

    
    matmul(c, d, r2, stripSize, n, bs);
    matmul(e, f, r3, stripSize, n, bs);
    matsum(r1, r2, r3, rf, stripSize, n, bs);
  }

  if (rank == COORDINATOR) commTimes[2] = MPI_Wtime();

  // recolectar suma
  MPI_Gather(rf, n*stripSize, MPI_DOUBLE, rf, n*stripSize, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
  if (rank == COORDINATOR) commTimes[3] = MPI_Wtime();

  MPI_Finalize();

  // ------------------------ CHECK ---------------------------------

  if (rank==COORDINATOR) {

		// Check results
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				check = check && (rf[i*n+j] == 3*n);

		if(check) printf("Multiplicacion de matrices resultado correcto\n");
		else printf("Multiplicacion de matrices resultado erroneo\n");

    // total time
    totalTime = commTimes[3] - commTimes[0];
    commTime = (commTimes[1] - commTimes[0]) + (commTimes[3] - commTimes[2]);

    printf("Multiplicacion (N=%d, bs=%d)\tTiempo total=%lf\tTiempo comm=%lf\n",n,bs,totalTime,commTime);
	}

  // ------------------------ FREE ------------------------------------

  free(a);
	free(b);
	free(c);
  free(d);
  free(e);
  free(f);
  free(r1);
  free(r2);
  free(r3);
  free(rf);

	return 0;

}