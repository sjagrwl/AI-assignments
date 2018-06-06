/*
    AI Assignment 2
    Sahaj Agarwal - 201601077
    Sanyem Gupta - 201601082
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct Network{

    double *weights;
    double errordelta;
    double value;
}NETWORK;

//Building Neural Network.
NETWORK** buildNetwork(int layers[],int nl){

    NETWORK** network=(NETWORK **)malloc(nl*sizeof(NETWORK*));
    int i,j,k;
    for(i=0;i<nl;i++){

        network[i]=(NETWORK *)malloc((layers[i]+1)*sizeof(NETWORK));
    }
    for(i=1;i<nl;i++){

        for(j=0;j<=layers[i];j++){

            if(j==0){

                network[i][j].value=1.0;
                network[i][j].weights=NULL;
            }
            else{

                network[i][j].value=0.0;
                network[i][j].weights=(double *)malloc((layers[i-1]+1)*sizeof(double));
                for(k=0;k<=layers[i-1];k++){

                    network[i][j].weights[k]=0.1+(rand()/9999999999.0);
                }
            }
            network[i][j].errordelta=0.0;
        }
    }
    return network;
}

//Initialising Input layer for each batch.
void InputLayerInit(NETWORK** network,double batch[],int nfeatures){

    int i;
    for(i=0;i<=nfeatures;i++){

        if(i==0){

            network[0][i].value=1.0;
            network[0][i].weights=NULL;
        }
        else{
            network[0][i].value=batch[i-1];
            network[0][i].weights=NULL;
        }
        network[0][i].errordelta=0.0;
    }
}

//Sigmoid function
double sigmoid(double x){

    return 1.0/(1.0 + exp(0.0-x));
}

//Derivative of Sigmoid function
double sigmoidPrime(double x){

    return sigmoid(x)*(1-sigmoid(x));
}
//Activation values of nodes in the Neural Network.
void NeuralNetworkactivation(NETWORK** network,int layers[],int nl){

    int i,j,k;
    for(i=1;i<nl;i++){

        for(j=1;j<=layers[i];j++){

            double net=0.0;
            for(k=0;k<=layers[i-1];k++){

                if(i==1 || k==0){

                    net+=network[i][j].weights[k]*network[i-1][k].value;
                }
                else{

                    net+=network[i][j].weights[k]*sigmoid(network[i-1][k].value);
                }
            }
            network[i][j].value=net;
        }
    }
}

//BackPropagating Error through the network.
void errorBackPropogation(NETWORK** network,int layers[],int nl,int target[],int CrossEntropy){

    int i,j,k;
    for(i=nl-1;i>0;i--){

        for(j=1;j<=layers[i];j++){

            if(i==nl-1){

                if(CrossEntropy==1){
                    network[i][j].errordelta=(target[j-1]-sigmoid(network[i][j].value));    
                }
                else{
                    network[i][j].errordelta=((target[j-1]-sigmoid(network[i][j].value))*sigmoidPrime(network[i][j].value));
                }
            }
            else{

                double netdelta=0.0;
                for(k=1;k<=layers[i+1];k++){

                    netdelta+=(network[i+1][k].errordelta*network[i+1][k].weights[j]*sigmoidPrime(network[i][j].value));
                }
                network[i][j].errordelta=netdelta;
            }
        }
    }
}

//Updating Weights and Biases of the Neural Network.
void UpdateNetwork(NETWORK** network,int layers[],int nl,int target[],float eta,int CrossEntropy){

    //Calculating activation of nodes in the Neural Network.
    NeuralNetworkactivation(network,layers,nl);

    //BackPropogating Error.
    errorBackPropogation(network,layers,nl,target,CrossEntropy);
    
    //Updating Weights.
    int i,j,k;
    for(i=1;i<nl;i++){

        for(j=1;j<=layers[i];j++){

            for(k=0;k<=layers[i-1];k++){

                if(i==1 || k==0){
                    network[i][j].weights[k]+=eta*network[i-1][k].value*network[i][j].errordelta;
                }
                else{
                    network[i][j].weights[k]+=eta*sigmoid(network[i-1][k].value)*network[i][j].errordelta;
                }
            }
        }
    }
}

//Print Neural Network
void printNeuralNetwork(NETWORK** network,int layers[],int nl,double eta){

    int i,j,k;
    for(i=0;i<nl;i++){

        printf("LAYER %d:\n",i+1);
        for(j=0;j<=layers[i];j++){

            if(i!=0 && j!=0){
                
                printf("%lf-> ",sigmoid(network[i][j].value));
            }
            else{

                printf("%lf-> ",network[i][j].value);
            }
            if(network[i][j].weights!=NULL){

                for(k=0;k<=layers[i-1];k++){

                    if(i==1 || k==0){
                        printf("(%lf,%lf),",network[i][j].weights[k],(eta*network[i][j].errordelta*network[i-1][k].value));
                    }
                    else{
                        printf("(%lf,%lf),",network[i][j].weights[k],(eta*network[i][j].errordelta*sigmoid(network[i-1][k].value)));
                    }
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

//Reading dataset from a CSV file.
double** read_csv(char file[],int rows,int columns){

    FILE* fp=fopen(file,"r");
    double** dataset=(double **)malloc((rows-1)*sizeof(double *));
    int i,j;
    for(i=0;i<rows;i++){

        dataset[i]=(double *)malloc((columns-1)*sizeof(double));
    }
    int c,linezero=0,ir=0,ic=0,iv=0;
    char value[10];
    if(fp){

        while((c=getc(fp))!=EOF){

            if(linezero==0){

                if(c==10){

                    linezero=1;
                }
            }
            else{

                if(c==44 || c==10){
                 
                    value[iv]='\0';
                    dataset[ir][ic]=atoi(value);
                    ic++;
                    ic=ic%columns;
                    if(ic==0){

                        ir++;
                        ir=ir%rows;
                    }
                    iv=0;
                }
                else{

                    value[iv]=c;
                    iv++;
                    iv=iv%10;
                }
            }
        }
    }
    return dataset;
}

//Normalizing Dataset
//Min-Max Normalization
void normalizer1(double** dataset,int rows,int cols){

    int i,j;
    double* maxValues=(double *)malloc((cols-1)*sizeof(double));
    double* minValues=(double *)malloc((cols-1)*sizeof(double));
    for(i=0;i<cols-1;i++){

        maxValues[i]=-1.0;
        minValues[i]=dataset[0][i+1];
    }
    for(i=0;i<rows-1;i++){

        for(j=1;j<cols;j++){

            if(dataset[i][j] > maxValues[j-1]){

                maxValues[j-1]=dataset[i][j];
            }
            if(dataset[i][j] < minValues[j-1]){

                minValues[j-1]=dataset[i][j];
            }
        }
    }
    for(i=0;i<rows-1;i++){

        for(j=1;j<cols;j++){

            dataset[i][j]=((dataset[i][j]-minValues[j-1])/(maxValues[j-1]-minValues[j-1]));
        }
    }
}

//Mean and Std Normalization
void normalizer2(double** dataset,int rows,int cols){

    int i,j;
    double* meanValues=(double *)malloc((cols-1)*sizeof(double));
    double* stdValues=(double *)malloc((cols-1)*sizeof(double));
    for(i=0;i<cols-1;i++){

        meanValues[i]=0.0;
    }
    for(i=0;i<rows-1;i++){

        for(j=1;j<cols;j++){

            meanValues[j-1]+=dataset[i][j];
        }
    }
    for(i=0;i<cols-1;i++){

        meanValues[i]=(meanValues[i]/(rows*1.0)-1.0);
    }
    for(i=0;i<rows-1;i++){

        for(j=1;j<cols;j++){

            stdValues[j-1]+=(dataset[i][j]-meanValues[j-1])*(dataset[i][j]-meanValues[j-1]);
        }
    }
    for(i=0;i<cols-1;i++){

        stdValues[i]=sqrt((stdValues[i]/(rows*1.0)-1.0));
    }
    for(i=0;i<rows-1;i++){

        for(j=1;j<cols;j++){

            dataset[i][j]=(dataset[i][j]-meanValues[j-1])/stdValues[j-1];
        }
    }
}

//Check Stopping Criterion.
int checkStoppingCriteria(NETWORK** network,int layers[],int nl,double eta){

    int i,j,k;
    double weightError=0.0;
    double werr=0.0;
    for(i=1;i<=layers[nl-1];i++){

        for(k=0;k<=layers[nl-2];k++){

            if(k==0){
                werr=(eta*network[nl-1][i].errordelta*network[nl-2][k].value);
                weightError+= werr*werr;
            }
            else{
                werr=(eta*network[nl-1][i].errordelta*sigmoid(network[nl-2][k].value));
                weightError+= werr*werr;
            }
        }
    }
    weightError=sqrt(weightError/layers[nl-1]);
    if(weightError < 0.0001){
        return -1;
    }
    else{
        return 1;
    }
}

int main(){

    int layers[3]={16,5,10}; //number of nodes in each layer.
    int nl=3; //number of layers including hidden layer.
    int epoch=0; //Epoch.
   // int nepoch=100; //No of Epochs.
    int CrossEntropy=1; //0-Use Default Quadratic Cost Function. 1-Use Cross-Entropy Cost Function.
    double eta[]={0.15,0.005}; //Learning Rate for Quadratic and Cross-Entropy Cost Functions respectively.
    int nincrease=5; //No of increase iterations for Hidden Layer node count. 

    double **trainingSet,**testSet; //Training And Test Sets.
    char trainFile[]="/home/sahaj/Desktop/UG-2/AI_Class/assign02/trainData.csv";
    int trainFileRows=2217,trainFileCols=17;
    char testFile[]="/home/sahaj/Desktop/UG-2/AI_Class/assign02/testData.csv";
    int testFileRows=999,testFileCols=17;
    
    srand((unsigned int) time(NULL));

    //Assuming Dataset file is a Plain Text CSV file with first row as Field Headers and ',' as Field Delimiters. 
    trainingSet=read_csv(trainFile,trainFileRows,trainFileCols); //Reading Training Set.
    testSet=read_csv(testFile,testFileRows,testFileCols); //Reading Testing Set.

    //Normalizing Dataset
    normalizer2(trainingSet,trainFileRows,trainFileCols);
    normalizer2(testSet,testFileRows,testFileCols);

    int z;
    double *QuadAccuracy=(double *)malloc(nincrease*sizeof(double));
    double *CrossAccuracy=(double *)malloc(nincrease*sizeof(double));
    for(z=0;z<nincrease;z++){
    
        layers[nl-2]=5+z; //Increasing no of Hidden Layers.
        printf("No Of Hidden Layer Nodes: %d\n",layers[nl-2]);

        NETWORK** network=buildNetwork(layers,nl); //building Neural Network with the given layer vector.
        NETWORK** networkCross=buildNetwork(layers,nl); //building Neural Network with the given layer vector.

        **networkCross=**network;
        
        CrossEntropy=0;
        int k;
        for(k=0;k<2;k++){

            //batch - Array[16] with feature values
            //target - Array[10] with 1 assigned to the corresponding class, rest 0.
            double *batch=(double *)malloc(layers[0]*sizeof(double));
            int *target=(int *)malloc(layers[nl-1]*sizeof(int));
            
            epoch=0;
            //Training Neural Network.
            while(1){
            
                int i,j;
                for(i=0;i<trainFileRows-1;i++){
                    
                    for(j=0;j<layers[nl-1];j++){

                        target[j]=0;
                    }
                    for(j=0;j<trainFileCols;j++){

                        if(j==0){

                            target[(int)(trainingSet[i][j]-1)]=1;
                        }
                        else{

                            batch[j-1]=trainingSet[i][j];
                        }
                    }
                    //Each Update Iteration.
                    if(CrossEntropy==0){
                        InputLayerInit(network,batch,layers[0]);
                        UpdateNetwork(network,layers,nl,target,eta[CrossEntropy],CrossEntropy);
                        if(checkStoppingCriteria(network,layers,nl,eta[CrossEntropy])==-1){
                            break;
                        }
                    }
                    else{
                        InputLayerInit(networkCross,batch,layers[0]);
                        UpdateNetwork(networkCross,layers,nl,target,eta[CrossEntropy],CrossEntropy);
                        if(checkStoppingCriteria(networkCross,layers,nl,eta[CrossEntropy])==-1){
                            break;
                        }
                    }   
                }
                if(CrossEntropy==0){
                    if(checkStoppingCriteria(network,layers,nl,eta[CrossEntropy])==-1){
                        break;
                    }
                }
                else{
                    if(checkStoppingCriteria(networkCross,layers,nl,eta[CrossEntropy])==-1){
                        break;
                    }
                }
                epoch++;
            }
            //Testing Neural Network
            int i,j;
            double accuracy;
            int correct=0,incorrect=0;
            int maxClass;
            for(i=0;i<testFileRows-1;i++){
            
                for(j=1;j<testFileCols;j++){

                    batch[j-1]=testSet[i][j];
                }

                //Finding Activations for each class.
                if(CrossEntropy==0){
                    InputLayerInit(network,batch,layers[0]);
                    NeuralNetworkactivation(network,layers,nl);
                }
                else{
                    InputLayerInit(networkCross,batch,layers[0]);
                    NeuralNetworkactivation(networkCross,layers,nl);
                }
                
                double maxAct=-1.0;
                if(CrossEntropy==0){  
                    for(j=1;j<=layers[nl-1];j++){

                        if(sigmoid(network[nl-1][j].value) > maxAct){

                            maxAct=sigmoid(network[nl-1][j].value);
                            maxClass=j;
                        }
                    }
                }
                else{  
                    for(j=1;j<=layers[nl-1];j++){

                        if(sigmoid(networkCross[nl-1][j].value) > maxAct){

                            maxAct=sigmoid(networkCross[nl-1][j].value);
                            maxClass=j;
                        }
                    }
                }
                
                if(maxClass==((int)testSet[i][0])){

                    correct+=1;
                }
                else{

                    incorrect+=1;
                }
            }
            if(CrossEntropy==0){
                QuadAccuracy[z]=(correct*1.0/(correct+incorrect))*100.0; //Network Accuracy On Testing Set for Quadratic Cost Func.
                //printNeuralNetwork(network,layers,nl,eta[CrossEntropy]); //Print Neural Network
                printf("Quadratic Cost Function => ACCURACY: %lf\n",QuadAccuracy[z]);
                free(network);
            }
            else{
                CrossAccuracy[z]=(correct*1.0/(correct+incorrect))*100.0; //Network Accuracy On Testing Set for Cross-Entropy Func.
                //printNeuralNetwork(networkCross,layers,nl,eta[CrossEntropy]); //Print Neural Network
                printf("Cross-Entropy Function => ACCURACY: %lf\n",CrossAccuracy[z]);
                free(networkCross);
            }
            CrossEntropy=1;
        }
    }
    int *nhidden=(int *)malloc(nincrease*sizeof(int));
    for(z=0;z<nincrease;z++){

        nhidden[z]=5+z;
    }

    //Ploting Accuracy vs No of hidden layer nodes for each Cost Function. 
    char * commandsForGnuplot[] = {"set title \"minimizing weight error stopping criteria\"","set origin 0.0,0.0","set xlabel 'number of nodes in hidden layer'","set ylabel 'accuracy'", "plot 'data.temp' using 3:1 with linespoints title 'Quadratic Cost Function','data.temp' using 3:2 with linespoints lt rgb 'red' title 'Cross Entropy Cost'"};
    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i;
    for (i=0; i < nincrease; i++) {
 
        fprintf(temp, "%lf %lf %d \n", QuadAccuracy[i], CrossAccuracy[i],nhidden[i]); //Write the data to a temporary file
    }
    for (i=0; i < 5; i++) {
 
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }

    return 0;
}