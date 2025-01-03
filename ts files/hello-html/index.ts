export function getDescription() {
    return {
        description: "hello-html.",
        input: [
            {
                id: "response",
                displayName: "Response",
                description: "Response.",
                defaultValue: "response://",
                readonly: true,
                required: true,
                type: "OutputResource",
            },
        ],
        output: [],
    } as const satisfies ScriptDescription;
}

export async function execute(context: Context): Promise<Output> {
    const stream = await context.openWriteText(context.parameters.response, "utf-8", responseOptions);
    const writer = stream.getWriter();
    await writer.write(html);
    await writer.close();
}

const responseOptions = {
    headers: {
        "Content-Type": "text/html; charset=utf-8",
    },
};

const html = `<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Hello World!</title>
	</head>
	<body>
		<h1>Hello World!</h1>
	</body>
</html>
`;
