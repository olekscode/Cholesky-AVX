library(ggplot2)

# Source: http://www.cookbook-r.com/Manipulating_data/Summarizing_data/
## Gives count, mean, standard deviation, standard error of the mean, and confidence interval (default 95%).
##   data: a data frame.
##   measurevar: the name of a column that contains the variable to be summariezed
##   groupvars: a vector containing names of columns that contain grouping variables
##   na.rm: a boolean that indicates whether to ignore NA's
##   conf.interval: the percent range of the confidence interval (default is 95%)
summarySE <- function(data=NULL, measurevar, groupvars=NULL, na.rm=FALSE,
                      conf.interval=.95, .drop=TRUE) {
    library(plyr)

    # New version of length which can handle NA's: if na.rm==T, don't count them
    length2 <- function (x, na.rm=FALSE) {
        if (na.rm) sum(!is.na(x))
        else       length(x)
    }

    # This does the summary. For each group's data frame, return a vector with
    # N, mean, and sd
    datac <- ddply(data, groupvars, .drop=.drop,
      .fun = function(xx, col) {
        c(N    = length2(xx[[col]], na.rm=na.rm),
          mean = mean   (xx[[col]], na.rm=na.rm),
          sd   = sd     (xx[[col]], na.rm=na.rm)
        )
      },
      measurevar
    )

    # Rename the "mean" column    
    datac <- rename(datac, c("mean" = measurevar))

    datac$se <- datac$sd / sqrt(datac$N)  # Calculate standard error of the mean

    # Confidence interval multiplier for standard error
    # Calculate t-statistic for confidence interval: 
    # e.g., if conf.interval is .95, use .975 (above/below), and use df=N-1
    ciMult <- qt(conf.interval/2 + .5, datac$N-1)
    datac$ci <- datac$se * ciMult

    return(datac)
}

fname <- "../data/experiment_results.csv"

df <- read.csv(fname, header = FALSE)
names(df) <- c("method", "size", "time")

df$method <- factor(df$method, levels=c("CB", "CB_AVX"), labels=c("Without AVX", "With AVX"))

sdf <- summarySE(df, measurevar="time", groupvars=c("method", "size"))

ggplot(sdf, aes(x=size, y=time, colour=method)) +
       geom_errorbar(aes(ymin=time-se, ymax=time+se)) +
       geom_line() +
       geom_point() +
       labs(x="Number of rows", y="Seconds", colour="Method", title="Cholesky-Banachiewicz factorization")