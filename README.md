Elementary 0.3
==============

##Installation##

To install Elementary, go to Elementary_0.2, where you will see a Makefile.

First make the dependencies by:

    $ CC="<PATH_TO_CC>" make dep

where CC and CPP must some features of C++0x. We have successfully tested gcc/g++ 4.7.2 on linux and clang/clang++ on Mac 10.7. On Mac, you need to further specify 

    BUILD_PREFIX="--build=x86_64-apple-darwin10.0.0"

Then we build Elementary by:

    $ CC="<PATH_TO_CC>" CPP="<PATH_TO_CPP>" make

This will produce a binary called "ele" in the same folder. You need then to set up an environment variable to include ./lib/urcu/lib/:

    $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/urcu/lib/

##Examples: LDA##

To run LDA, you can type in:

    ./ele --app lda \
        --data ../Test/WebLDA/ap/ \
        --nepoch 3 \
        --output /tmp/out \
        --verbose_output \
        --lda_alpha 0.1 \
        --lda_beta 0.1 \
        --lda_ntopic 10


While most of these parameters are self-explanable, the output file specified by --output is a TSV file:

     <DOC_PATH>  <WORD>  <TOPIC>


