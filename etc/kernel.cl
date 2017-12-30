double rosenbrock(double * vals, int nvals) {
	double val = 0;
	
	for (unsigned int i = 0; i < nvals-1; i++) {
		double xi = vals[i];
		val += 100 * pown(vals[i+1] - pown(xi, 2), 2) + pown(1-xi, 2);
	}

	return val;
}
			
double schwefel( double * vals, int nvals) {
	double val = 418.9829*nvals;
	double sum = 0;
	
	//int j = (get_global_id(0)*nvals);
	
	for (int i = 0; i < nvals; i++) {
		double x = vals[i];
		sum += x * sin(sqrt(fabs(x)));
	}

	return val - sum;
}

void decode(double * vals, global char * genes, int numgenes, int numvars, double xmin, double step, int numbits) {
	int offset = 0;
	int w = (get_global_id(0)*numgenes);
	//int v = (get_global_id(0)*numvars);
	for (int j = 0; j < numvars; j++) {
		ulong valor = 0;
		ulong potencia = 1;
		for (int i = numbits + offset - 1; i >= offset; i--) {
			int bit = genes[ w + i];
			valor += bit * potencia;
			potencia *= 2;
		}
		
		offset += numbits;
		vals[j] = (step * valor) + xmin;
	}
}

void grayToBinary(global char * genes, int numgenes) {
	int j = (get_global_id(0)*numgenes);
	for (int i = 1; i < numgenes; i++) {
		if (!genes[j + i]) {
			genes[j + i] = genes[j + i - 1];
		} else {
			genes[j + i] = !genes[j + i - 1];
		}
	}
}
		
void kernel evaluate(global char* genes, global const double* args, global double* fitness) {

	// load arguments
	int numgenes = args[0];   // numgenes
	double xmin = args[1];    // xmin
	double step = args[2];    // step
	double numbits = args[3]; // numbits
	int numvars = args[4];    // numvars
	int function = args[5];    // function
	
	// buffer to keep inermedia calcs this buffer fix max number of vars
	double vars[100];
	
	grayToBinary(genes, numgenes);
	decode(&vars, genes, numgenes, numvars, xmin, step, numbits); 
	double f = schwefel(&vars, numvars);
	
	switch (function) {
		case 1: {
			f = schwefel(vars, numvars);
			break;
		}
		case 5: {
			f = rosenbrock(vars, numvars);
			break;
		}
	} 	
	
	fitness[get_global_id(0)] = 1.0 / (1.0 + f);
}

