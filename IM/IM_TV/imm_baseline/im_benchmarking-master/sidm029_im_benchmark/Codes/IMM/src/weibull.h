

struct Weibull{
    static double f[1000][10000];
    static void init(){
        static int inited = false;
        if (inited)
            return;

        inited = true;
        for(int i=1 ;i < 1000; i++){
            for(int j=1; j<10000; j++){
                f[i][j] = pow(-log(i/1000.0), 1.0/(j/1000.0));
            }
        }
        return;
            //double randWeibull= scale * f[int(randDouble*1000)][int(shape*1000)];
    }
};
double Weibull::f[1000][10000];
