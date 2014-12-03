#pragma once

namespace DCInjDn
{
	/// <summary>
    /// Implement this interface in types to inject.
    /// </summary>
	public interface struct IInjectable
	{
		/// <summary>
        /// Called when the type is injected.
        /// </summary>
        void OnLoad();

		/// <summary>
        /// Command called from injecting process, or other.
        /// </summary>
        /// <param name="buffer">Byte array of data passed into this process. 
		/// The content of this buffer can be changed and will be passed back to the caller.</param>
        void OnCommand(array<unsigned char>^ buffer );

		/// <summary>
        /// Called when the type is ejected.
        /// </summary>
        void OnUnload();

	}; // public interface struct

} // namespace VInjDn
