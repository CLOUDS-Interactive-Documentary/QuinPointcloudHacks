# QuinPointcloudHacks

Please refer to [EmptyVisualSystem](https://github.com/CLOUDS-Interactive-Documentary/EmptyVisualSystem) for instructions on how to create a Visual System.

The heart of this Visual System is a shader which contains a few different sets of functionality. There are a number of flags which are used to enable and disable the different processing paths on the vertex and fragment shader.

First is a "3D Printing" effect, which gradually transitions points to larger high-contrast, low-saturation points. This happens a row at a time, slowly "building" the pointcloud into a more solid form.

Second is a shattering effect. This effect pushes all the vertices away from the surface of the speaker. This is combined with Audio Amplitude information to make the person "congeal" as they speak and drift back into an abstract cloud as they pause.

Third is a vibration effect. This reacts to the Audio Amplitude as well and runs the sine wave "through" the vertices, displacing them proportionally to the volume.

### Included Addons

CLOUDS uses many addons from the community 

* [ofxRange](https://github.com/Flightphase/ofxRange) by James George

* [ofxMSATimer](https://github.com/obviousjim/ofxMSATimer) by Memo Atken, James George, Philip Whitfield, Juan Pablo Manson
		
* [ofxTween](https://github.com/Flightphase/ofxTween) by Arturo Castro, James George

* [ofxTextInputField](https://github.com/Flightphase/ofxTextInputField) by James George & Elliot Woods
		
* [ofxTimecode](https://github.com/YCAMInterlab/ofxTimecode) by James George
		
* [ofxTimeline](https://github.com/YCAMInterlab/ofxTimeline) by James George, supported by YCAMInterlab
	
* [ofxCameraSaveLoad](https://github.com/roymacdonald/ofxCameraSaveLoad) by Roy MacDonald
		
* [ofxGenerative](https://github.com/rezaali/ofxGenerative) by Reza Ali
		
* [ofxUI](https://github.com/rezaali/ofxUI) by Reza Ali

### Loading Data

Anywhere you want to access things in your bin/data folder you'll need to do so by first calling 

    getVisualSystemDataPath() + "path/to/my/stuff"
    
This will ensure that your data access works when running the module inside of CLOUDS.

### Saving Presets

CLOUDS VisualSystems are driven by presets. To create a preset, configure the variables and timing just how you like and then click Save on the main GUI. Enter a name on the popup window and confirm. This will save all your settings into bin/data/Presets/

The Presets GUI now has a button with the name you just entered, and you can click it to bring these settings back. Feel free to change anything after you've created a preset, since you'll always be modifying the Working/ preset.

Make sure to commit your presets so everyone can check them out!

### Hot-Keys 

Some useful hotkeys for the interface
* F : Toogle Fullscreen
* H : Hide/Show GUI
* E : Arrange GUI in Cascade  
* R : Arrange GUI at Top
* T : Minimize GUI
* Y : Arrange GUI in circle
* ` : Take a snapshoot
* SPACE : Toggly play on the timeline
* SHIFT+T : Set a camera point on the camera track
* SHIFT+L : Toggle following your camera track

