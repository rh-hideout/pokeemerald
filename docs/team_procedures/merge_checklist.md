# Document Purpose

This document is a guide for maintainers to account for all the reccomended steps before merging in a pull request.
<!-- Here's an optional markdown checklist version that you can post in your reviews. -->
<!-- https://files.catbox.moe/nqxvnl.md -->

# Checklist

## Is the branch's theoretical functionality in scope?
If you're not sure if a branch's functionality is [in scope](docs/team_procedures/scope.md), start a conversation on Discord to resolve.

## Does the branch successfully compile?
From `make clean`, the branch should locally compile.

## Do all CI tests pass?
Contributors are asked to make sure tests pass locally, but maintainers should at least wait for the CI to pass before merging.

## Have you verified that the functionality works in game without any problems?
If functionality cannot be verified with an automated test, proof of an in game test is required. Do not be afraid to reach out to the contributor or the community to make sure something works in game as it should.

## If the branch ports behavior from another Pokémon game, have you verified that the behavior functions as faithfully as possible? 
We have always tried to make sure we can mimic the original functionality as closely as possible so as to avoid confusion with users and players. Do not be afraid to ask the contributor / community for proof if you cannot personally verify.

## If this branch changes a function that is expected to be modified by users, is there a migration script?
Not everything needs a migration script - if you're unsure, start a discussion.

## Should new functionality introduced by this branch be gated behind a config?
We don't have a strict definition of when configs should be used, but you can start with 

> Why SHOULDN'T this be a config?

## Are tests written for everything that can be tested?
If you're not sure if something CAN be tested, start a discussion. Some contributors may not be capable of writing tests - we should guide them in #expansion-tests to do so.

If any new tests are `KNOWN_FAILING`, issues should be opened describing each of the `KNOWN_FAILING` tests and our understanding of why they fail.

## Does the branch meet our [config philosophy](docs/styleguide#config-philosophy)?

## Does the branch meet our [saves philosophy](docs/styleguide#saves-philosophy)?

## Does the submitted code follow the [styleguide](docs/styleguide)?
This applies to code that comes from other branches or games.

## Is the pull request appropriately labeled?
Without labels, the CHANGELOG will not be properly formatted. For specifically the `bugfix` label, an additional label, detailing what area the bug exists in is required.

## Is `pokeemerald-expansion` free from a merge freeze?
Our [release schedule](docs/team_procedures/schedule.md) prevents us from merging Big Features and non-bugfixes within certain dates close to a release. Please use `/release` in the RHH Discord to clarify when these are occuring.

