### NAME
nntrain - nn4cc training app

### USAGE
      nntrain [OPTIONS] DATAFILES...

### DESCRIPTION
Nntrain loads data files and trains the neural network on the training
set, while simultaneously evaluating performance on testing sets in
log-scheduled time steps.  Now compiled for 146-26-3 neural networks.

Data files should be in the SSV form.  First inputs columns, then
outputs columns, then ignored data until the end of line.  Hash
delimited comments are ignored.  Filenames are either train.dat,
test.dat, test1.dat and test2.dat, or any names with role specified
by the -train and -test options.

Training output is saved into the ./Log/ directory.

### OPTIONS
               -h  This help.
               -v  Extra verbose mode.
               -q  Quiet recompilation.
              -np  No progress report.
  
          -n NAME  Sets the experiment name for logdir and journal.log.
                   Patterns %d,%t are formatted to datetime and topology.
                   If no NAME is specified the datetime is used.
           -train  All data files after this switch will be a training set.
            -test  Following files are testing set (alias -test1).
           -test2  Following files are alternative testing set.
  
           -e NUM  The number of training epochs/cycles (default no limit),
        -secs NUM  or the desired length of the training in seconds,
        -pats NUM  or the desired number of patterns to train.  Whichewer will
                   happen first.  Decimal values for epochs or secs are accepted.
  
           -g NUM  Learning rate, gamma (default 0.08).
      -t TOPOLOGY  Topology (current is 146-26-3).
  
           -p NUM  The number of testing threads running in parallel.
           -s NUM  Logarithmic scheduler step multiplier (default 1.3).
                   Inputs 20% or 0.2 are converted to the 1.2 multiplier.
              -dt  Deterministic tests order (default is random order).
  
              -E0  No errors logging.
              -E1  Default, aggregated MAE and its per-pat min/max log.
              -E2  Advanced errors log with MSE, distance, per output MAEs
                   and complete outputs logs.
              -E3  Median-type errors.
              -W0  Don't log weights.
              -C1  Log credits, simple input weights aggregates.

### TOPOLOGY
          10-30-3  10 inputs, 30 hiden neurons, 3 outputs.
        4-2sigm-3  4 inputs, 2 sigmoidal hidden, 3 (default sigm.) outputs.
     5-2r-3s-4r-6  5 inputs, 2 relu, 3 sigm., 4 relu, 6 default sigm.

### STOP
    single Ctrl-C  Finish current error evaluations and stop.
    double Ctrl-C  Close all error evaluations and stop.
                   Queued, but not started evaluations will be lost anyway.

### EXAMPLE
      nntrain -secs 5 -t 146-26-3 train.dat

### VERSION
nn4cc-0.2 (c) R.Jaksa 2019, GPLv3

