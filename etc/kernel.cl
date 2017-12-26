double schwefel(global double * vals, int nvals) {
	double val = 0;
	for (int i = 0; i < nvals; i++) {
		double sum = 0;
		for (int j = 0; j <= i; j++ ) {
			sum += vals[(get_global_id(0)*nvals) + j];
		}
		val += pown(sum, 2);
	}

	return val;
}

void decode(global double * vals, global char * genes, int numgenes, int numvars, double xmin, double step, int numbits) {
	int offset = 0;
	
	for (int j = 0; j < numvars; j++) {
		int valor = 0;
		int potencia = 1;
		for (int i = numbits + offset - 1; i >= offset; i--) {
			int bit = genes[(get_global_id(0)*numgenes) + i];
			valor += bit * potencia;
			potencia *= 2;
		}
		
		offset += numbits;
		vals[(get_global_id(0)*numvars)+j] = (step * valor) + xmin;
	}
}

void grayToBinary(global char * genes, int numgenes) {
	// aux buffer TODO this buffer marks the maximum numgenes to use
	char binary[4096];
	binary[0] = genes[get_global_id(0)*numgenes];
	
	for (int i = 1; i < numgenes; i++) {
		if (!genes[(get_global_id(0)*numgenes) + i]) {
			binary[i] = binary[i-1];
		} else {
			binary[i] = !binary[i-1];
		}
	}
	
	// copy the buffer
	for (int i = 0; i < numgenes; i++) {
		genes[(get_global_id(0)*numgenes) + i] = binary[i];
	}
}
		
void kernel evaluate(global char* genes, global const double* args, global double* fitness, global double* vars) {

	// load arguments
	int numgenes = args[0];   // numgenes
	double xmin = args[1];    // xmin
	double step = args[2];    // step
	double numbits = args[3]; // numbits
	int numvars = args[4];    // numvars
	
	grayToBinary(genes, numgenes);
	decode(vars, genes, numgenes, numvars, xmin, step, numbits); 
	double f = schwefel(vars, numvars);
	
	fitness[get_global_id(0)] = 1.0 / (1.0 + f);
}
