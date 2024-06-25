pipeline {
    agent any

    stages {
        stage('Configure') {
            steps {
                echo 'Configure Bangor build'
                sh   'cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=DEBUG -DCODE_COVERAGE=ON'
            }
        }
        stage('Build') {
            steps {
                echo 'Build Bangor containers'
                sh   'cmake --build build-coverage --verbose'
            }
        }
        stage('Tests') {
            steps {
                echo 'Execute Bangor containers tests'
                sh   'cd build-coverage; ctest'
                sh   'cd build-coverage; xcrun llvm-profdata merge -o Tests_Containers.profdata default.profraw'
                sh   'cd build-coverage; xcrun llvm-cov show --instr-profile Tests_Containers.profdata libbgcontainers.a'
            }
        }
    }
}
