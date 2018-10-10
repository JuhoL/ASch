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
                script {
                    currentBuild.result = 'SUCCESS'
                }
            }
        }

        stage ('Tests') {
            when {
                expression {
                    currentBuild.result == 'SUCCESS'
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
            when {
                expression {
                    currentBuild.result == 'SUCCESS'
                }
            }
            steps {
                echo 'Devise test report...'
            }
        }
    }
}
