# Nodeflux-Assessment
 C++ Software Engineer Nodeflux Assessment   
   
   
   
   
## How to build :

```
git clone https://github.com/farhannz/Nodeflux-Assessment.git
cd Nodeflux-Assessment
docker build --pull --rm -f "Dockerfile" -t nodefluxassessment:latest "." 
docker run --rm -it  -p 3000:3000/tcp nodefluxassessment:latest 
```