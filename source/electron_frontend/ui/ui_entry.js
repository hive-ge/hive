const { desktopCapturer } = require("electron");

(async function () {
    //Get stream from the game engine.

    try {

        const data = await desktopCapturer.getSources({ types: ['window'] });

        console.log({ data });


        function handleStream(stream) {
            debugger;
            const video = document.createElement("video");
            video.srcObject = stream;
            video.onloadedmetadata = (e) => video.play();
            document.body.appendChild(video);
        }

        function handleError(error) {
            console.error(error);
        }

        console.log(data);
        for (const source of data) {
            if (source.name == "Steam") {

                const stream = await navigator.mediaDevices.getUserMedia({
                    audio: false,
                    video: {
                        mandatory: {
                            chromeMediaSource: 'desktop',
                            chromeMediaSourceId: source.id,
                            minWidth: 1280,
                            maxWidth: 1920,
                            minHeight: 720,
                            maxHeight: 1080
                        }
                    }
                });
                handleStream(stream);
            }
        }

    } catch (e) {
        console.log("Could not setup capture of window");
        console.error(e);
    }

})();
console.log(desktopCapturer);