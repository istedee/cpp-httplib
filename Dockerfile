FROM llvm/llvm-project:latest
  
RUN apt-get update && apt-get install -y git cmake

RUN git clone https://github.com/istedee/cpp-httplib.git

COPY fuzz_target.cpp /cpp-httplib/
  
WORKDIR /cpp-httplib

RUN clang++ -fsanitize=fuzzer,address -I . fuzz_target.cpp -o httplib_fuzzer

RUN apt-get clean && rm -rf /var/lib/apt/lists/*

CMD ["./httplib_fuzzer"]
