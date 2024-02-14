build-test-binary:
	@go build -mod=readonly -o ./build/cmd-main -ldflags="-s -w" ./cmd/main.go
