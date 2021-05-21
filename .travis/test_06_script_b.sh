#!/usr/bin/env bash
#
# Copyright (c) 2018 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

#deploy test builds
if [ "$DEPLOY_TEST_BUILDS" = "true" ] && [ "$BUILD_ONLY_DEPENDS" = "false" ]; then
  BEGIN_FOLD deploybins
  if [ "$REASON" = "MacOS" ]; then DOCKER_EXEC pwd && find . -name '*.dmg' && echo $OUTDIR && mkdir -p "$TRAVIS_BUILD_DIR/uccrelease" && cp *.dmg "$TRAVIS_BUILD_DIR/uccrelease/NWO-$REASON-$TRAVIS_BRANCH.dmg" && cd "$TRAVIS_BUILD_DIR/uccrelease" && ls; fi
  if [ "$REASON" != "MacOS" ]; then DOCKER_EXEC pwd && mkdir -p "$TRAVIS_BUILD_DIR/uccrelease/NWO-$REASON-$TRAVIS_BRANCH" && find $OUTDIR && cd $OUTDIR && pwd && ls -Rha && cp -a bin/* "$TRAVIS_BUILD_DIR/uccrelease/NWO-$REASON-$TRAVIS_BRANCH/" && cd "$TRAVIS_BUILD_DIR/uccrelease" && strip NWO-$REASON-$TRAVIS_BRANCH/* && ls NWO-$REASON-$TRAVIS_BRANCH/* && zip -r NWO-$REASON-$TRAVIS_BRANCH.zip * && ls; fi
  if [ "$ADDWINSETUP" = "true" ]; then DOCKER_EXEC cd "$TRAVIS_BUILD_DIR/release" && pwd && cp -a *.exe "$TRAVIS_BUILD_DIR/uccrelease/" && cd "$TRAVIS_BUILD_DIR/uccrelease" && strip *.exe && ls -Rha && zip -r NWO-$REASON-$TRAVIS_BRANCH.zip * && ls; fi
  DOCKER_EXEC git init
  DOCKER_EXEC git config --global user.email "3713548+flyinghuman@users.noreply.github.com"
  DOCKER_EXEC git config --global user.name "Travis-User"
  DOCKER_EXEC git add --force --all
  DOCKER_EXEC git commit -m "Latest-Build"
  DOCKER_EXEC git remote add origin https://github.com/flyinghuman/ucc-builds.git
  DOCKER_EXEC git push -f -u https://$BUILDTOKEN@github.com/flyinghuman/ucc-builds.git master:$REASON
  END_FOLD
fi
