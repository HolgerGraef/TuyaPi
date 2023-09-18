.PHONY: codeformat
codeformat:
	find src \
	-regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' \
	-exec \
		clang-format -style=file -i {} \;

.PHONY: codeformat-check
codeformat-check:
	rm -f clang-format-error
	find src \
		-regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' \
		-exec bash -c "clang-format -style=file -n --Werror {} || touch clang-format-error" \;
	@if [ -f clang-format-error ]; then \
		echo "Code formatting error"; \
		rm clang-format-error; \
		exit 1; \
	else \
		echo "Code formatting OK"; \
		exit 0; \
	fi

.PHONY: coverity
coverity:
	@if [ -z "${TOKEN}" ]; then \
		echo "TOKEN is not set"; \
		exit 1; \
	fi
	if [ -z "${EMAIL}" ]; then \
		echo "EMAIL is not set"; \
		exit 1; \
	fi

	cd build && \
		make clean && \
		qmake .. && \
		/opt/coverity/bin/cov-build --dir cov-int make -j4 && \
		tar -czvf ../build.tar.gz .

	curl --form token=${TOKEN} \
		--form email=${EMAIL} \
		--form file=@build.tar.gz \
		--form version="v0.2.0" \
		--form description="TuyaPi - Home Automation with Tuya and Raspberry Pi" \
		https://scan.coverity.com/builds?project=hgrf%2FTuyaPi
