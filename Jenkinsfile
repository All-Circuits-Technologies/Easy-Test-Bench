// SPDX-FileCopyrightText: 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

pipeline {
    agent {
        node {
            label 'TestBedDeb64'
        }
    }

    options {
        // Keep only logs of latest 5 builds for each branch,
        // and only keep artifacts of latest build of each branch
        buildDiscarder logRotator(daysToKeepStr: '',
                                  numToKeepStr: '5',
                                  artifactDaysToKeepStr: '',
                                  artifactNumToKeepStr: '1')
    }

    stages {

        stage('Checkout') {
            steps {
                cleanWs()
                checkout scm
                sh 'git clean -dfx'
            }   
        }
        
        stage('QMake') {  
            steps {
                sh 'mkdir __build'
                sh 'cd __build/ && qmake CONFIG+=release ..'
            }       
        }
        
        stage('Make') {
            steps {
                sh 'cd __build/ && make -j $(nproc || echo 1)'
            }
        }
        
        stage('Install') {
            steps {
                sh 'mkdir __install'
                sh 'cd __build/ && make install INSTALL_ROOT=${PWD}/../__install/'
            }
        }
        
        stage('Translate') {
            steps {
                sh 'find ./ -name "*.ts" -execdir lrelease "{}" \\;'
                sh 'find ./ -name "*.qm" -exec mv "{}" __install/opt/testbed \\;'
            }
        }

        stage('Test') {
            steps {             
                sh 'find __install/opt/testbed/ -type f -executable -name "test-bed-utest-*" "!" -name "test-bed-utest-serial-link" "!" -name "test-bed-utest-bmsgadgetjsonloader" -print0 | xargs -0 -n 1 sh -c'
            }
        }

        stage('Package') {
            steps {
                sh 'cd __install/ && tar czf testbed-linux${BRANCH_NAME:+-${BRANCH_NAME##*/}}-$(git describe --dirty || git rev-parse --short --verify HEAD || echo "${CHANGE_ID:-${BUILD_ID:-unknown}}").tgz opt/'
                sh 'cd __install/opt/testbed && tar czf ../../testbed-translations${BRANCH_NAME:+-${BRANCH_NAME##*/}}-$(git describe --dirty || git rev-parse --short --verify HEAD || echo "${CHANGE_ID:-${BUILD_ID:-unknown}}").tgz *.qm'
                archiveArtifacts artifacts: '__install/testbed-*.tgz', fingerprint: true
            }
        }
    }
}
