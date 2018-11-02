library('FiRE')
source('utils/preprocess.R')

mkdirs <- function(fp) {
    if(!file.exists(fp)) {
        mkdirs(dirname(fp))
        dir.create(fp)
        }
    }

#Create output directory
outputFolder = 'output/R/jurkat'
mkdirs(outputFolder)

#Read data
preprocessedData <- read.table(gzfile('data/preprocessedData_jurkat_two_species_1580.txt.gz'))
preprocessedData <- as.matrix(preprocessedData) #Samples * Features

#Read Labels
labels <- read.table('data/labels_jurkat_two_species_1580.txt') #Cells with label '1' represent abundant, while cells with label '2' represent rare.

#Create model of FiRE.
model <- new(FiRE::FiRE, 100, 50, 1017881, 5489, 0)
model$fit(preprocessedData)

#Assign FiRE score to every cell.
score <- model$score(preprocessedData)
write.csv(score,paste(outputFolder,'/score_R.txt',sep=""), quote=F)

#Apply IQR-based criteria to identify rare cells for further downstream analysis.
q3 <- quantile(score, 0.75)
iqr <- IQR(score)
th <- q3 + (1.5*iqr)

#Select indexes that satisfy IQR-based thresholding criteria.
indIqr <- which(score >= th)

#Create a file with binary predictions
predictions <- integer(dim(preprocessedData)[1])
predictions[indIqr] <- 1 #Replace predictions for rare cells with '1'.
write.csv(predictions,paste(outputFolder,'/predictions_R.txt',sep=""), quote=F)




