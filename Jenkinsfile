def testVariable = 1

pipeline {
    agent any

    stages {
        stage('Checkout') {
            steps {
                echo 'Checkout'
            }
        }

        stage('Build') {
            steps {
                echo 'Build'
                def buildResult = 'SUCCESS'
            }
        }

        stage ('Tests') {
            when {
                expression {
                    buildResult == 'SUCCESS'
                }
            }
            parallel {
                stage('Unit tests') {
                    steps {
                        echo 'Run unit tests...'
                    }
                }
                stage('Static analysis') {
                    steps {
                        echo 'Run static analysis...'
                    }
                }
            }
        }

        stage('Test report') {
            steps {
                echo 'Devise test report...'
            }
        }
    }
}
