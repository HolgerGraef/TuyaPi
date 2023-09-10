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
