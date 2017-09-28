unit MultiMediaReg;

interface

uses Windows, Classes;

procedure Register;

implementation

uses MediaPlayView;

procedure Register;
begin
  RegisterComponents('WSControl', [TMediaPlayerView]);
end;

end.
