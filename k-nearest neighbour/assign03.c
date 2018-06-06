#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Reading dataset from a plain text file in 1 Fold.
double** read_data(char file[],int rows,int columns){

    FILE* fp=fopen(file,"r");
    double** dataset=(double **)malloc((rows)*sizeof(double *));
    int i,j;
    for(i=0;i<rows;i++){

        dataset[i]=(double *)malloc((columns)*sizeof(double));
    }
    int c,linezero=1,ir=0,ic=0,iv=0;
    char value[10];
    if(fp){

        while((c=getc(fp))!=EOF){

            if(linezero==0){

                if(c==10){

                    linezero=1;
                }
            }
            else{

                if(c==32 || c==10){
                
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

//Reading dataset from a plain text file in k Folds.
double*** read_data_cross(char file[],int rows,int columns,int kfold){

    FILE* fp=fopen(file,"r");
    double*** dataset=(double ***)malloc((kfold)*sizeof(double **));
    int i,j,k;
    int rowsPerFold=ceil((rows*1.0)/kfold);
    for(i=0;i<kfold;i++){

        dataset[i]=(double **)malloc(rowsPerFold*sizeof(double *));
        for(j=0;j<rowsPerFold;j++){

            dataset[i][j]=(double *)malloc(columns*sizeof(double));
        }
    }
    int c,linezero=1,ifold=0,ir=0,ic=0,iv=0;
    char value[10];
    if(fp){

        while((c=getc(fp))!=EOF){

            if(linezero==0){

                if(c==10){

                    linezero=1;
                }
            }
            else{

                if(c==32 || c==10){
                
                    value[iv]='\0';
                    dataset[ifold][ir][ic]=atoi(value);
                    ic++;
                    ic=ic%columns;
                    if(ic==0){

                        ifold++;
                        ifold=ifold%kfold;
                        if(ifold==0){

                            ir++;
                            ir=ir%rowsPerFold;
                        }
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
        while(ifold!=0){

            for(i=0;i<columns;i++){

                dataset[ifold][ir][i]=-1.0;
            }
            ifold++;
            ifold=ifold%kfold;
        }
    }
    return dataset;
}

//Calculating Euclidean Distance btw vector1 and vector2
double euclideanDistance(double* vector1,double* vector2,int length){

    int i;
    double distance=0.0;
    for(i=0;i<length;i++){

        distance+=(vector1[i]-vector2[i])*(vector1[i]-vector2[i]);
    }
    distance=sqrt(distance);
    return distance;
}

//Sorting KNN array
void getSortedKNN(double** KNN,int length){

	int i,j;
    double key,label;
    for(i=1;i<length;i++){
       
       key=KNN[i][0];
       label=KNN[i][1];
       j=i-1;
       while(j>=0 && KNN[j][0]>key){
           
           KNN[j+1][0]=KNN[j][0];
           KNN[j+1][1]=KNN[j][1];
           j-=1;
       }
       KNN[j+1][0]=key;
       KNN[j+1][1]=label;
   	}
}

//Returns predicted label Of Test Instance on Training Set Using k-NNC with k <- kOpt.
//crossVal => 0:Don't Cross Validate, 1:Cross Validate - excludeFold <- Fold to exclude for testing.
int PredictLabelUsingKNNC(double*** trainingSet,double* testRow,int kOpt,int kfold,int rowsPerFold,int cols,int nLabel,int crossVal,int excludeFold){

    //KNN => (EucDistance (double),label (double)) , Size => k+1;
    int i,j,k;
    double** KNN=(double **)malloc((kOpt+1)*sizeof(double *));
    for(i=0;i<kOpt+1;i++){

        KNN[i]=(double *)malloc(2*sizeof(double));
    }
    //Initialize KNNs to (-1.0,-1.0)    
    for(i=0;i<kOpt+1;i++){

        KNN[i][0]=-1.0;
        KNN[i][1]=-1.0;
    }

    //Calculate KNNs for Test Instance
    double distance=0.0;
    k=0;
    for(i=0;i<kfold;i++){

        if(!crossVal || (i!=excludeFold)){
        
            for(j=0;j<rowsPerFold;j++){

                if(trainingSet[i][j][cols-1]!=-1.0){

                    //Calculating Euclidean distance.
                    distance=euclideanDistance(testRow,trainingSet[i][j],cols-1);
                    
                    //Finding kNNs on training set.
                    if((i==0 && j<=kOpt)||(excludeFold==0 && i==1 && j<=kOpt)){
                    
                        KNN[k][0]=distance;
                        KNN[k][1]=trainingSet[i][j][cols-1];
                        k++;
                        if(k==(kOpt+1)){

                            getSortedKNN(KNN,kOpt+1);
                        }
                    }
                    else{

                        if(distance < KNN[kOpt][0]){

                            KNN[kOpt][0]=distance;
                            KNN[kOpt][1]=trainingSet[i][j][cols-1];
                            getSortedKNN(KNN,kOpt+1);
                        }
                    }
                }
            }
        }
    }

    //Predicting label on the basis of maximum count of NNs of a given class.
    double* labelCount=(double *)malloc(nLabel*sizeof(double));
    double max=0.0;
    int flag=1;
    int maxNNlabel=-1;
    k=kOpt-1;
    for(i=0;i<nLabel;i++){

        labelCount[i]=0.0;
    }
    if(kOpt!=1){
     
        while(k>=1 && KNN[k][0]==KNN[k-1][0]){

            k--;
        }
        if(k==0){

            flag=0;
        }
    }
    for(i=0;i<kOpt;i++){

        if(kOpt!=1 && flag==1){

            labelCount[(int)KNN[i][1]]+=((KNN[kOpt-1][0]-KNN[i][0])/(KNN[k][0]-KNN[0][0]));
        }
        else{

            labelCount[(int)KNN[i][1]]+=1.0;
        }
    }
    for(i=0;i<nLabel;i++){

        if(labelCount[i] > max){

            max=labelCount[i];
            maxNNlabel=i;
        }
    }

    return maxNNlabel;
}

//Testing k-NNC on testing set.
double testingKNNC(double*** trainingSet,double** testSet,int kOpt,int kfold,int rowsPerFold,int testFileRows,int testFileCols,int nLabel,int crossVal,int excludeFold){

    int i,label;
    int correct=0,incorrect=0;
    float accuracy,error;
    for(i=0;i<testFileRows;i++){

        if(testSet[i][testFileCols-1]!=-1.0){
        
            //Predicting Label Of Test Instance on Training Set Using k-NNC with k <- kOpt.
            label=PredictLabelUsingKNNC(trainingSet,testSet[i],kOpt,kfold,rowsPerFold,testFileCols,nLabel,crossVal,excludeFold);
            if(((int)testSet[i][testFileCols-1])==label){

                correct+=1;
            }  
            else{

                incorrect+=1;
            }
        }
    }
    accuracy = ((correct*1.0)*100.0)/(correct+incorrect); //Calculating accuracy of k-NNC

    return accuracy;
}

//Cross Validation to find optimum value  of parameter k in k-NNC
int CrossValidation(double*** trainingSet,int kmin,int kmax,int kfold,int rowsPerFold,int cols,int nLabel){

    int i,j,k;
    int kOpt;
    double* error=(double *)malloc((kmax-kmin+1)*sizeof(double));
    double err=0.0;
    for(k=kmin;k<=kmax;k++){

        for(j=0;j<kfold;j++){

            //Testing k-NNC on validation set.
            err=testingKNNC(trainingSet,trainingSet[j],k,kfold,rowsPerFold,rowsPerFold,cols,nLabel,1,j);
            error[k-1]+=(100.0-err);
        }
        //Avg. Error
        error[k-1]=error[k-1]/kfold; 
        printf("Avg. Error Rate for k=%d: %lf\n",k,error[k-1]);
    }

    double minerror=-1.0;
    for(i=kmin-1;i<kmax;i++){

        if(minerror==-1.0){

            minerror=error[i];
            kOpt=i+1;
        }
        else if(error[i]<minerror){

            minerror=error[i];
            kOpt=i+1;
        }
    }

    return kOpt;
}

int main(){

    int kfold = 3;// Specifying kfold-Cross Validation.

    double ***trainingSet,**testSet; //Training And Test Sets.
    char trainFile[]="/home/sahaj/Desktop/UG-2/AI_Class/assign03/pp_tra.dat";
    int trainFileRows=6670,trainFileCols=193;
    char testFile[]="/home/sahaj/Desktop/UG-2/AI_Class/assign03/pp_tes.dat";
    int testFileRows=3333,testFileCols=193;

    int nLabel=10; //No. of classes

    //Assuming Dataset file is a Plain Text file with last column as Class Label and ' ' as Field Delimiters. 
    trainingSet=read_data_cross(trainFile,trainFileRows,trainFileCols,kfold); //Reading Training Set.
    testSet=read_data(testFile,testFileRows,testFileCols); //Reading Testing Set.
    
    int kmin=1,kmax=25; //Specifying range of k values for k-NNC
    int rowsPerFold=ceil((trainFileRows*1.0)/kfold);
    int kOpt=1; //Specifying Default k value for k-NNC
    kOpt=CrossValidation(trainingSet,kmin,kmax,kfold,rowsPerFold,trainFileCols,nLabel);

    printf("Optimal k: %d\n",kOpt);

    //Testing k-NNC on testing set.
    double accuracy = testingKNNC(trainingSet,testSet,kOpt,kfold,rowsPerFold,testFileRows,testFileCols,nLabel,0,-1); //Calculating accuracy of k-NNC
    double error = 100.0 - accuracy; //Calculating Error Rate for k-NNC
    
    printf("ACCURACY: %f\n",accuracy);
    printf("Error Rate: %f\n",error);

    free(trainingSet);
    free(testSet);

    return 0;
}